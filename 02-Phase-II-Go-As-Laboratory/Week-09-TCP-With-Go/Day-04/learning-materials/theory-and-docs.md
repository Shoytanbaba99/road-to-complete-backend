Pull up your chair, let’s look at the screen together.

Today is **Week 9, Day 4: Timeouts and Connection Shutdown**.

Yesterday, we solved the concurrency problem by giving each client its own goroutine. You saw Client A and Client B talking to your server at the same time without blocking each other.

Now look at your code from yesterday with an attacker's mindset.

What happens if I write a Python script that opens 10,000 TCP connections to your server, finishes the 3-way handshake, and then **sends absolutely nothing**?

In yesterday's code, each connection spawned a goroutine parked on `scanner.Scan()`. That call will sit there waiting for bytes _forever_. The operating system holds an open file descriptor in its FD table, Go keeps a parked goroutine in its runtime scheduler, and memory quietly fills up. This is a classic **Slowloris / Idle-Connection Denial of Service (DoS)** attack.

Without timeouts, any random machine on the internet can choke your server by simply refusing to speak.

---

### The Two Deadly Traps: Half-Open Sockets and Wall-Clock Skew

Before writing a single line of timeout logic, we need to understand what the Linux network stack is doing under the hood.

#### 1. The Half-Open Socket Problem

Imagine Client A connects over Wi-Fi. While connected, the user walks out of range or unplugs the Ethernet cable.

Did the client send a TCP `FIN` packet? **No.**
Did the client send an `RST` packet? **No.**

The physical link vanished into thin air. From the perspective of your Linux server, the TCP socket is still in the `ESTABLISHED` state. Unless you configure TCP Keep-Alives (which can take two hours by default on Linux) or enforce an application-level deadline, **your server will never know the client is dead**. That socket descriptor will leak and stay open until your server reboots.

#### 2. Wall-Clock Time vs. Deadlines

Back in Week 2, Day 6, you studied monotonic clocks vs. wall-clock time (`CLOCK_REALTIME`). If you try to implement timeouts by doing manual math with `time.Now()` and checking the time on every loop, an NTP clock sync (which can step time backwards) can break your calculations.

Go solves this directly at the runtime network poller level using **Deadlines**.

---

### How Go Handles Timeouts: `SetDeadline`

Every `net.Conn` provides three critical methods:

```go
conn.SetDeadline(t time.Time)      // Absolute point in time for both Read and Write
conn.SetReadDeadline(t time.Time)  // Absolute point in time for future Read calls
conn.SetWriteDeadline(t time.Time) // Absolute point in time for future Write calls

```

Notice the argument: it is **`time.Time`**, an absolute point in time, _not_ a duration like `time.Duration(5 * time.Second)`.

#### The Golden Rule: The Deadline is NOT a Countdown Timer

A deadline is an absolute date on the calendar.

If you write:

```go
conn.SetReadDeadline(time.Now().Add(5 * time.Second))

```

That means: _"Any `Read()` that happens after 5 seconds from right now must fail with a timeout error."_

If the client sends a message after 2 seconds, `Read()` succeeds. But the deadline is **still set** to the original expiration time! If the client tries to send a second message 4 seconds later, `Read()` will immediately blow up because the original deadline already passed.

Therefore, for persistent connections, you must **slide the deadline forward** on every successful read:

```text
 Client connects:
   SetReadDeadline = NOW + 5s ────┐
                                  │ (Client sends "HELLO" after 2s)
 Successful Read:                 ▼
   Re-arm: SetReadDeadline = NOW + 5s ────┐
                                          │ (Client sends "WORLD" after 1s)
 Successful Read:                         ▼
   Re-arm: SetReadDeadline = NOW + 5s ────┐
                                          │ (Client goes silent for 6s...)
                                          ▼
 Read() unblocks with Timeout Error! ────> Close connection & kill goroutine.

```

---

### Detecting Timeouts vs. Real Network Errors

When a deadline expires, `conn.Read()` or `scanner.Scan()` does not return `io.EOF`. It returns an error that implements the `net.Error` interface.

You can inspect whether an error was caused by a timeout:

```go
if netErr, ok := err.(net.Error); ok && netErr.Timeout() {
    // This connection was terminated because the client was idle too long
}

```

---

### The Other Half: Half-Close (`CloseWrite` vs. `CloseRead`)

When you call `conn.Close()`, your OS sends a `RST` or a full bidirectional `FIN` tearing down both directions of the connection immediately.

But TCP actually supports **simplex shutdown** (Half-Close):

- You can shut down the **write** half of the connection: you send a `FIN` to tell the client _"I am done sending you data,"_ but you keep your read channel open to hear whatever the client has left to say.
- In Go, a raw `net.Conn` can be type-asserted to a `*net.TCPConn` to access `CloseWrite()` and `CloseRead()`:

```go
if tcpConn, ok := conn.(*net.TCPConn); ok {
    tcpConn.CloseWrite() // Sends TCP FIN packet, but allows continued reads
}

```

---

### Hands-on Code: The Resilient TCP Server

Let’s write the code for Day 4. We will build a TCP server that enforces a **5-second idle timeout**. If a client connects and stays silent for more than 5 seconds, the server warns them, terminates the socket, and reclaims the goroutine.

Create `main.go`:

```go
package main

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"log"
	"net"
	"runtime"
	"strings"
	"time"
)

const IdleTimeout = 5 * time.Second

func main() {
	addr := "127.0.0.1:9002"
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("failed to listen on %s: %v", addr, err)
	}
	defer listener.Close()

	log.Printf("Timeout-protected server running on %s", addr)

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("accept error: %v", err)
			continue
		}

		go handleClient(conn)
	}
}

func handleClient(conn net.Conn) {
	// Guarantee that socket file descriptor is closed when goroutine exits
	defer conn.Close()

	remoteAddr := conn.RemoteAddr().String()
	log.Printf("[+] Connect: %s | Goroutines: %d", remoteAddr, runtime.NumGoroutine())

	greeting := fmt.Sprintf("Connected. You will be kicked if idle for more than %v.\n", IdleTimeout)
	if _, err := conn.Write([]byte(greeting)); err != nil {
		return
	}

	reader := bufio.NewReader(conn)

	for {
		// 1. Arm/Re-arm the deadline before every read
		err := conn.SetReadDeadline(time.Now().Add(IdleTimeout))
		if err != nil {
			log.Printf("[-] Failed to set deadline for %s: %v", remoteAddr, err)
			return
		}

		// 2. Block waiting for input or deadline expiration
		line, err := reader.ReadString('\n')
		if err != nil {
			// Check if this error was a timeout
			var netErr net.Error
			if errors.As(err, &netErr) && netErr.Timeout() {
				log.Printf("[!] Idle timeout reached for %s. Dropping connection.", remoteAddr)
				conn.Write([]byte("ERROR: Connection timed out due to inactivity. Goodbye.\n"))
				return
			}

			if errors.Is(err, io.EOF) {
				log.Printf("[-] Clean disconnect (EOF) from %s", remoteAddr)
				return
			}

			log.Printf("[-] Read error from %s: %v", remoteAddr, err)
			return
		}

		// Process incoming message
		text := strings.TrimSpace(line)
		if text == "" {
			continue
		}

		if text == "QUIT" {
			conn.Write([]byte("BYE\n"))
			return
		}

		log.Printf("[%s]: %s", remoteAddr, text)
		response := fmt.Sprintf("ACK: %s (Deadline reset)\n", text)
		if _, err := conn.Write([]byte(response)); err != nil {
			log.Printf("[-] Write error to %s: %v", remoteAddr, err)
			return
		}
	}
}

```

---

### Terminal Verification Drill

Do this right now across two terminals:

#### Test 1: Active Communication

1. **Terminal 1:** Run `go run main.go`.
2. **Terminal 2:** Connect with `nc 127.0.0.1 9002`.
3. Type `hello` within 2 seconds. You will see:

```text
ACK: hello (Deadline reset)

```

4. Type `still here` within another 3 seconds. The deadline is successfully sliding forward on every read.

#### Test 2: The Idle Timeout Drop (The Defense)

1. In Terminal 2, stop typing completely and watch the clock.
2. Exactly 5 seconds after your last message, notice what happens in Terminal 2:

```text
ERROR: Connection timed out due to inactivity. Goodbye.

```

The socket closes, and `nc` returns back to your shell prompt. 3. Look at Terminal 1 (Server):

```text
[!] Idle timeout reached for 127.0.0.1:xxxxx. Dropping connection.

```

4. The goroutine exited cleanly and did not leak.

---

### Today's Micro-Capstone

**Objective:**
Add an explicit **Graceful Teardown** mechanic to this server.

**Requirements:**

1. Keep the 5-second `SetReadDeadline` active so idle clients are kicked.
2. Add a `SHUTDOWN` command:

- When a client types `SHUTDOWN`:
- The server writes a farewell banner: `"Shutting down write channel...\n"`.
- Use a type assertion `tcpConn, ok := conn.(*net.TCPConn)` to call **`tcpConn.CloseWrite()`**.
- Notice: `CloseWrite()` sends a `FIN` packet to the client. The client can no longer receive data from the server, but the server can still read from the client until the client sends their own `FIN` (clean two-stage TCP half-close).

3. Test it with netcat and observe how `CloseWrite()` signals the client side.

Run the code, test the 5-second idle drop, and test the `SHUTDOWN` half-close. Let me know what you see on the terminal.
