## Week 9, Day 3 (Re-anchored): Concurrent Client Handling & Goroutines

Forget the network for two minutes. Look at what happens inside the machine when code executes.

### 1. The Physical Reality: OS Threads vs. Goroutines

Back in Week 1 (Day 3) and Week 2 (Day 1), you looked at processes and threads.

When you write a multi-threaded C program and call `pthread_create()`, here is what Linux does:

1. It executes the `clone` system call with flags like `CLONE_VM`, `CLONE_FS`, `CLONE_FILES`, `CLONE_SIGHAND`.
2. The kernel allocates a `task_struct` in kernel memory.
3. The kernel reserves a chunk of virtual address space for that thread's stack—by default on Linux, this is **2 MB to 8 MB**.
4. The Linux kernel scheduler (`CFS` / Completely Fair Scheduler) now tracks this thread alongside every other thread in the OS.

#### The Cost of the OS Model

- **Memory footprint:** If you want 10,000 concurrent client connections, and each thread takes an 8 MB stack:

$$10,000 \times 8\text{ MB} = 80\text{ GB of virtual memory}$$

Your machine swaps, runs out of memory, and the kernel OOM killer murders the process.

- **Context switch cost:** When the CPU switches from Thread A to Thread B at the OS level:
- CPU traps into Ring 0 (Kernel Mode).
- Saves hardware registers to memory.
- Modifies the CPU's control registers (like `CR3` on x86 if switching address spaces, or kernel scheduler bookkeeping).
- Flushes or pollutes CPU L1/L2 data and instruction caches.
- Switches back to Ring 3 (User Mode).
- **Time cost:** ~1,000 to 1,500 nanoseconds.

#### The Go Model: User-Space Tasks

A goroutine is **not** an OS thread. The Linux kernel has no idea what a goroutine is. If you run `ps -T -p <pid>` on a Go program running 10,000 goroutines, Linux will often only report 4, 8, or 16 actual OS threads.

- **Stack Size:** A goroutine starts with a tiny stack: roughly **2 KB** (2,048 bytes). If a function needs more room (deep recursion, large local arrays), the Go runtime dynamically allocates a larger block on the heap, copies the stack over, adjusts the pointers, and frees the old small stack.
- **Context Switch Cost:** When switching between goroutines:
- No system call.
- No Ring 0 kernel trap.
- It simply saves 3 CPU registers (Program Counter `PC`, Stack Pointer `SP`, and the `DX` context register) in user-space memory, loads the registers for the next goroutine, and jumps.
- **Time cost:** ~10 to 20 nanoseconds.

---

### 2. The Engine Under the Hood: The G-M-P Scheduler

How do thousands of these 2 KB tasks actually hit the CPU? Through Go’s internal runtime scheduler, known as the **G-M-P model**:

```text
               THE RUNTIME ENGINE (User Space)

      [ G1 ]  [ G2 ]  [ G3 ]  [ G4 ]  [ G5 ]   <-- Goroutines (Tasks: 2 KB initial stack)
        │       │       │       │       │
        └───────┴───────┼───────┴───────┘
                        │
                        ▼ (Local Run Queue)
                 [ P: Processor ]              <-- Logical Context (GOMAXPROCS = Num CPU Cores)
                        │
                        ▼
                 [ M: Machine ]                <-- OS Thread (Created via clone syscall)
                        │
 ═══════════════════════╪═══════════════════════════ Linux Kernel Boundary (Ring 0)
                        ▼
                 [ CPU Core ]                  <-- Physical Silicon

```

- **G (Goroutine):** Represents the execution state (program counter, stack pointer, status flags). It is a struct (`runtime.g`) living in Go's user-space memory.
- **M (Machine):** A real operating system thread created by the Go runtime via `clone()`. It executes code on a physical CPU core.
- **P (Processor / Logical Context):** The resource required to execute Go code. The number of `P`s is determined by `runtime.GOMAXPROCS(0)` (which defaults to the number of logical CPU cores on your machine).

#### What Happens During Network I/O? (The Netpoller)

This is where the magic happens for backend engineering.

In standard C, if an OS thread calls `read(fd, buf, size)` on a blocking socket, the kernel suspends that entire OS thread. The thread cannot do anything until bytes arrive from the wire.

In Go:

1. Goroutine `G1` runs on OS Thread `M1` and calls `conn.Read()`.
2. No bytes are available on the socket yet.
3. The Go runtime intervenes: it does **not** let the OS thread sleep.
4. Instead, the runtime registers the socket file descriptor with the OS kernel's I/O event notification mechanism (**`epoll`** on Linux).
5. The runtime marks `G1` as "waiting" and parks it off to the side.
6. OS Thread `M1` immediately grabs `G2` from the local run queue and starts executing it. The thread never sits idle!
7. When bytes finally arrive from the network card, `epoll` wakes up Go's background network poller thread, which moves `G1` back into a runnable queue, ready to be picked up by the next available `M`.

---

### 3. The Minimal Day 3 Reality: The Concurrent Echo Server

We do not need channels, hubs, or broadcast rooms to master Day 3.

The goal of Day 3 is simple: **Take yesterday's serial server and allow multiple clients to connect, hold connections open, and communicate independently without blocking each other.**

Look at the difference:

#### The Yesterday Problem (Serial)

```go
for {
    conn, err := listener.Accept()
    if err != nil {
        continue
    }

    // Execution enters here and STALLS.
    // Accept() cannot be called again until this client disconnects!
    handleConnection(conn)
}

```

#### The Day 3 Solution (Concurrent)

```go
for {
    conn, err := listener.Accept()
    if err != nil {
        log.Printf("Accept error: %v", err)
        continue
    }

    // The 'go' keyword tells the runtime scheduler:
    // "Allocate a 2 KB G-struct, point its PC to handleConnection,
    // put it on the run queue, and return immediately."
    go handleConnection(conn)
}

```

The accept loop does not wait for `handleConnection` to finish. It loops back to `listener.Accept()` in less than a microsecond, ready for the next client.

---

### Hands-on Code: Day 3 Micro-Capstone

Here is the exact, unbloated code for Day 3. Notice we add **`runtime.NumGoroutine()`** so you can physically watch the runtime spawn and destroy goroutines as clients connect and disconnect.

```go
// main.go
package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"net"
	"runtime"
	"strings"
	"time"
)

func main() {
	addr := "127.0.0.1:9999"
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("Failed to bind: %v", err)
	}
	defer listener.Close()

	log.Printf("Server listening on %s", addr)
	log.Printf("[Diagnostics] Initial Goroutines: %d", runtime.NumGoroutine())

	// Background ticker: prints active goroutines every 5 seconds
	go func() {
		for {
			time.Sleep(5 * time.Second)
			log.Printf("[Diagnostics] Active Goroutines in memory: %d", runtime.NumGoroutine())
		}
	}()

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("Accept error: %v", err)
			continue
		}

		// Launch a concurrent goroutine per client
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	// Crucial: ensure the socket closes when this goroutine returns
	defer conn.Close()

	remoteAddr := conn.RemoteAddr().String()
	log.Printf("[+] Connect: %s | Goroutines: %d", remoteAddr, runtime.NumGoroutine())

	// Send an initial greeting to the client
	conn.Write([]byte("Connected to Concurrent Echo Server. Type commands or QUIT.\n"))

	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}

		if line == "QUIT" {
			conn.Write([]byte("BYE\n"))
			break // Breaks loop, hits defer conn.Close(), function exits
		}

		// Echo the line back to this specific client
		response := fmt.Sprintf("ECHO: %s\n", line)
		if _, err := conn.Write([]byte(response)); err != nil {
			break
		}
	}

	if err := scanner.Err(); err != nil && err != io.EOF {
		log.Printf("[-] Read error from %s: %v", remoteAddr, err)
	}

	log.Printf("[-] Disconnect: %s | Goroutines remaining: %d", remoteAddr, runtime.NumGoroutine()-1)
}

```

---

### Your Terminal Verification Drill

Do this right now across three terminal tabs to see the scheduler in action:

1. **Terminal 1 (Server):**

```bash
go run main.go

```

Note the initial goroutine count (usually 2: `main` + your background ticker). 2. **Terminal 2 (Client A):**

```bash
nc 127.0.0.1 9999

```

- Watch Terminal 1: Goroutine count increments by 1.
- Do **not** type anything. Let Client A sit idle, blocked on input.

3. **Terminal 3 (Client B):**

```bash
nc 127.0.0.1 9999

```

- Watch Terminal 1: Goroutine count increments again.
- Type `hello from client B`. Notice that Client B gets an immediate response even though Client A is completely idle!
- Yesterday, Client B would have been frozen out. Today, they execute concurrently.

4. **Verify OS Sockets & Threads:**
   Open a 4th tab and run:

```bash
# Check the TCP connections at the kernel level
ss -tan state established '( sport = :9999 )'

```

You will see both sockets in `ESTAB` state, managed concurrently. 5. **Kill Client B (`Ctrl+C` or type `QUIT`):**

- Watch Terminal 1: The goroutine finishes its function, exits cleanly, and the goroutine count drops back down.

Run this. Observe the goroutine lifecycle on your screen. When you've confirmed that independent execution works without blocking the accept loop, we take the next step.
