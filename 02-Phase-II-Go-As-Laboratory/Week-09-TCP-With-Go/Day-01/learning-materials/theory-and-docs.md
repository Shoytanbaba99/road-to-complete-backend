Yes. Today is **Week 9, Day 1: `net.Listen`, Sockets, and the Accept Loop**.

You are officially crossing from file-based I/O into network programming.

In Week 2 and Week 3, you inspected sockets from the outside using `ss`, `netcat`, and `strace`. Today, you write the Go code that commands the Linux kernel to allocate a socket, bind it to a port, and begin accepting incoming connections.

---

### The OS Foundation: What Happens Under the Hood

When you want to accept incoming network connections on Linux, your program does not talk directly to the network card (NIC). It talks to the Linux kernel via three sequential system calls:

```text
1. socket()  ---> Allocates a socket file descriptor (an integer in your FD table).
2. bind()    ---> Binds the FD to an IP address and Port (e.g., 127.0.0.1:8080).
3. listen()  ---> Marks the socket as "passive" (ready to accept incoming connections)
                  and creates a queue for pending TCP handshakes.

```

Once `listen()` succeeds, remote clients can perform the **TCP 3-Way Handshake** (SYN $\rightarrow$ SYN-ACK $\rightarrow$ ACK) with your kernel. The kernel handles the handshake entirely in Ring 0 and places the completed connection into an **Accept Queue**.

Your Go program then calls:

```text
4. accept()  ---> Pops the next completed connection off the queue and returns
                  a BRAND NEW file descriptor for that specific client.

```

Notice the distinction:

- **The Listening Socket:** Stays on the port, never exchanges application data, and only creates new client connections.
- **The Connected Socket:** Exists solely to read and write bytes with that specific remote client.

---

### How Go Models This: `net.Listen` and `net.Conn`

Go’s standard library collapses `socket()`, `bind()`, and `listen()` into a single function:

```go
listener, err := net.Listen("tcp", ":8080")

```

- `"tcp"` tells Go to use IPv4/IPv6 TCP sockets (`SOCK_STREAM`).
- `":8080"` binds to port `8080` on all available network interfaces (`0.0.0.0` / `::`).

`net.Listen` returns a `net.Listener` interface:

```go
type Listener interface {
    Accept() (Conn, error)
    Close() error
    Addr() Addr
}

```

The key method is `Accept()`. Calling `listener.Accept()` blocks execution until a client connects. Once a handshake completes, it returns a `net.Conn`.

Here is the essential bridge to Week 8:
**A `net.Conn` implements `io.Reader` and `io.Writer`.**

Because a socket is just a stream of bytes, every skill you practiced with `bufio.Scanner`, `io.Copy`, and `json.NewDecoder` works identically across a network cable.

---

### The Architecture: The Accept Loop

A server cannot accept one connection and terminate. It must run an infinite loop to continually pop clients off the kernel backlog:

```text
       ┌───────────────────────┐
       │ net.Listen("tcp", ...)│
       └──────────┬────────────┘
                  │
                  ▼
        ┌──────────────────┐
  ┌────►│ listener.Accept()│ <── (Blocks until client completes 3-way handshake)
  │     └─────────┬────────┘
  │               │ Returns net.Conn
  │               ▼
  │     ┌──────────────────┐
  │     │  Handle client   │
  │     │   (Read/Write)   │
  │     └─────────┬────────┘
  │               │
  │               ▼
  │     ┌──────────────────┐
  │     │ conn.Close()     │
  │     └─────────┬────────┘
  │               │
  └───────────────┘ Loop repeats

```

---

### Complete Implementation: A Single-Client TCP Server

Create a file named `main.go`:

```go
package main

import (
	"fmt"
	"io"
	"net"
	"os"
)

func main() {
	addr := "127.0.0.1:9000"

	// 1. Ask kernel for a passive listening socket on port 9000
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		fmt.Fprintf(os.Stderr, "failed to bind on %s: %v\n", addr, err)
		os.Exit(1)
	}
	defer listener.Close()

	fmt.Printf("Listening for TCP connections on %s...\n", addr)

	// 2. The Accept Loop
	for {
		// Accept() blocks until a TCP 3-way handshake completes
		conn, err := listener.Accept()
		if err != nil {
			fmt.Fprintf(os.Stderr, "failed to accept connection: %v\n", err)
			continue
		}

		// Handle the connection synchronously (one at a time for Day 1)
		handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	// Always close the client socket when done to avoid FD leaks
	defer conn.Close()

	remoteAddr := conn.RemoteAddr().String()
	fmt.Printf("[+] New connection established from %s\n", remoteAddr)

	// Write a greeting back to the client over the raw socket
	greeting := "Welcome to raw TCP. Type something and hit enter.\n"
	_, err := conn.Write([]byte(greeting))
	if err != nil {
		fmt.Printf("[-] Write error to %s: %v\n", remoteAddr, err)
		return
	}

	// Read incoming bytes into a fixed buffer
	buf := make([]byte, 1024)
	for {
		n, err := conn.Read(buf)
		if err != nil {
			if err == io.EOF {
				fmt.Printf("[-] Client %s disconnected cleanly (EOF)\n", remoteAddr)
			} else {
				fmt.Printf("[-] Read error from %s: %v\n", remoteAddr, err)
			}
			break
		}

		// Print received bytes to standard output
		fmt.Printf("[%s says]: %s", remoteAddr, string(buf[:n]))
	}
}

```

---

### Inspecting Sockets from the Terminal

Run the program in one terminal tab:

```bash
go run main.go

```

Open a second terminal tab and use the tools from Week 3 to inspect the operating system state:

#### 1. Verify the listening socket exists

Run `ss` to look for listening TCP sockets on port `9000`:

```bash
ss -tlnp sport = :9000

```

Output:

```text
State      Recv-Q Send-Q Local Address:Port  Peer Address:PortProcess
LISTEN     0      128        127.0.0.1:9000       0.0.0.0:*    users:(("main",pid=...,fd=3))

```

- **`LISTEN`**: The socket state.
- **`Send-Q 128`**: The maximum size of the kernel accept backlog.
- **`fd=3`**: File Descriptor `3` is the listener.

#### 2. Connect a raw TCP client

Use `nc` (netcat) to establish the TCP connection:

```bash
nc 127.0.0.1 9000

```

You will receive:

```text
Welcome to raw TCP. Type something and hit enter.

```

Type `hello from terminal` and hit Enter. Look back at your server tab:

```text
[+] New connection established from 127.0.0.1:54322
[127.0.0.1:54322 says]: hello from terminal

```

Press `Ctrl+C` in the `nc` terminal. The client sends a TCP `FIN` packet, which causes `conn.Read()` on the server to return `io.EOF`, triggering clean socket closure.

---

### Today's Micro-Capstone

**Objective:**
Build and observe a simple TCP listener that captures connection metadata and cleanly terminates connections.

**Requirements:**

1. Write a program that listens on `127.0.0.1:8888`.
2. In the accept loop, print:

- Local address (`conn.LocalAddr()`)
- Remote peer address (`conn.RemoteAddr()`)

3. When a client connects:

- Immediately write back an ASCII banner containing the client's own IP/port.
- Read data until the client closes the connection.
- Count the total number of bytes read from that client during the session and print it when they disconnect.

4. **The Synchronous Trap Experiment:**

- Run the server.
- Open two separate terminal tabs and connect two netcat clients at the same time:
- Tab 2: `nc 127.0.0.1 8888`
- Tab 3: `nc 127.0.0.1 8888`

- Type in Tab 3. Notice what happens (or doesn't happen) while Tab 2 remains open.

This experiment will demonstrate why synchronous accept loops cannot scale to multiple clients, laying the groundwork for Day 2 (Framing) and Day 3 (Goroutine concurrency).
