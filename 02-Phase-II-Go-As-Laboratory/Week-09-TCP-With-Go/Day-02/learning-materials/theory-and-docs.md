Today is **Week 9, Day 2: Read/Write Bytes, The Framing Problem, and Newline-Delimited Protocols**.

Yesterday, you read bytes using a fixed-size buffer:

```go
buf := make([]byte, 1024)
n, err := conn.Read(buf)

```

It looked like it worked because you typed one line into `nc`, pressed Enter, and read that exact line. But that was an illusion created by your slow human typing. Under real conditions, that code will silently corrupt application messages.

---

### The Fundamental Illusion: Packets vs. Streams

Developers coming to TCP often carry a dangerous misconception:

> _"If the client calls `conn.Write([]byte("Hello"))`, the server will receive `"Hello"` in its first `conn.Read()`."_

**This is completely false.**

- UDP has **message boundaries**. One `sendto()` = one datagram packet = one `recvfrom()`.
- TCP has **no concept of messages**. TCP is an unbroken, continuous **stream of bytes**.

To TCP, your application data is just water flowing through a pipe. TCP does not know or care about words, lines, or JSON payloads.

---

### The Framing Problem: Packet Splitting & Coalescing

Between your client's `Write()` and your server's `Read()`, two physical network behaviors occur constantly:

#### 1. Coalescing (The Nagle Algorithm / Packet Merging)

The client sends three separate messages in quick succession:

```text
Write("GET /index.html")
Write("GET /style.css")
Write("GET /app.js")

```

Because network packets have header overhead (20 bytes IP + 20 bytes TCP), the OS network stack may buffer these small writes and combine them into a single Ethernet frame.

When the server calls `conn.Read(buf)`:

```text
Received in ONE Read(): "GET /index.htmlGET /style.cssGET /app.js"

```

If your server assumed one `Read()` equals one command, it treats all three requests as one mangled command and crashes or errors out.

#### 2. Fragmentation (Splitting / MTU Limits)

The client writes a 4,000-byte JSON string:

```text
Write(largePayload) // 4000 bytes

```

The Ethernet MTU (Maximum Transmission Unit) is typically **1,500 bytes**. The TCP stack fragments that payload into multiple TCP segments:

- Segment 1: 1,460 bytes
- Segment 2: 1,460 bytes
- Segment 3: 1,080 bytes

If the server calls `conn.Read(buf)` with a 1,024-byte buffer:

- Read 1 gets: First 1,024 bytes (incomplete JSON)
- Read 2 gets: Next 1,024 bytes
- Read 3 gets: Next 1,024 bytes
- Read 4 gets: Final 928 bytes

If you pass `buf[:n]` directly to `json.Unmarshal`, it throws a syntax error on Read 1 because the JSON document is chopped in half.

---

### The Solution: Application-Layer Framing

Because TCP provides zero message boundaries, **your application must define the boundary**.

There are three common ways protocols solve this:

| Framing Strategy    | How It Works                                                               | Examples                                             |
| ------------------- | -------------------------------------------------------------------------- | ---------------------------------------------------- |
| **Delimiter-Based** | Messages end with a special sentinel byte sequence (e.g., `\n` or `\r\n`). | SMTP, Redis RESP (simple strings), IRC, HTTP headers |
| **Length-Prefixed** | Every message starts with a fixed-size integer indicating payload length.  | TLS records, gRPC / Protocol Buffers, HTTP/2 frames  |
| **Fixed-Size**      | Every message is exactly $N$ bytes long. Unused space is zero-padded.      | Legacy financial feeds, raw telemetry                |

Today, we focus on the most intuitive framing strategy: **Delimiter-based framing (Newline-delimited, `\n`)**.

---

### Connecting Week 8 to Week 9: Enter `bufio.Reader` and `bufio.Scanner`

You already mastered `bufio` for reading files line-by-line in Week 8. Because `net.Conn` implements `io.Reader`, we can wrap a socket directly in a `bufio.Reader` or `bufio.Scanner`.

`bufio.Reader` handles the heavy lifting:

1. It maintains an internal buffer (typically 4 KB).
2. It reads large chunks of bytes from the socket in as few `read()` syscalls as possible.
3. It scans memory for the delimiter byte (`'\n'`).
4. If a message is cut in half across network segments, it automatically waits for more bytes until the delimiter arrives before yielding the message to your application.

---

### Implementation: The Framing Dilemma Illustrated

Here is a server that uses `bufio.Reader` to properly frame messages separated by `\n`:

```go
package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"strings"
)

func main() {
	listener, err := net.Listen("tcp", "127.0.0.1:9001")
	if err != nil {
		panic(err)
	}
	defer listener.Close()

	fmt.Println("Framing server listening on 127.0.0.1:9001...")

	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("Accept error:", err)
			continue
		}

		handleClient(conn)
	}
}

func handleClient(conn net.Conn) {
	defer conn.Close()

	remoteAddr := conn.RemoteAddr().String()
	fmt.Printf("[+] Client connected: %s\n", remoteAddr)

	// Wrap the raw net.Conn (io.Reader) in a buffered reader
	reader := bufio.NewReader(conn)

	for {
		// ReadSlice / ReadString buffers bytes until it encounters '\n'
		line, err := reader.ReadString('\n')
		if err != nil {
			if err == io.EOF {
				fmt.Printf("[-] Client %s disconnected cleanly\n", remoteAddr)
			} else {
				fmt.Printf("[-] Read error from %s: %v\n", remoteAddr, err)
			}
			return
		}

		// Strip newline / carriage returns for clean processing
		payload := strings.TrimRight(line, "\r\n")

		// Process framed message
		fmt.Printf("[%s] Full message received: %q\n", remoteAddr, payload)

		// Echo the framed message back with a trailing delimiter
		response := fmt.Sprintf("ACK: %s\n", payload)
		if _, err := conn.Write([]byte(response)); err != nil {
			fmt.Printf("[-] Write error: %v\n", err)
			return
		}
	}
}

```

---

### Demonstrating the Framing Problem with a Test Client

To see why this matters, write a client that deliberately fires multiple writes into the socket without pauses:

```go
// client.go
package main

import (
	"fmt"
	"net"
	"time"
)

func main() {
	conn, err := net.Dial("tcp", "127.0.0.1:9001")
	if err != nil {
		panic(err)
	}
	defer conn.Close()

	// Write three messages immediately without delay
	// Over raw TCP, these may arrive in a single read buffer on the server
	conn.Write([]byte("MSG_ONE\n"))
	conn.Write([]byte("MSG_TWO\n"))
	conn.Write([]byte("MSG_THREE\n"))

	time.Sleep(100 * time.Millisecond)
}

```

If your server used yesterday's `conn.Read(buf)` with a single read, it would capture `"MSG_ONE\nMSG_TWO\nMSG_THREE\n"` all at once.
With `bufio.Reader` using `ReadString('\n')`, the server cleanly extracts:

```text
[127.0.0.1:...] Full message received: "MSG_ONE"
[127.0.0.1:...] Full message received: "MSG_TWO"
[127.0.0.1:...] Full message received: "MSG_THREE"

```

---

### The Denial-of-Service (DoS) Trap with Delimiters

What happens if a malicious client connects and writes 500 megabytes of raw text **without ever sending a `\n**`?

`reader.ReadString('\n')` will continue allocating memory in heap buffers, waiting indefinitely for the newline, until your server runs out of RAM and crashes (OOM).

In production, delimiter-based readers must enforce a **maximum message length limit**. You can use `bufio.Scanner` with `scanner.Buffer(buf, maxCapacity)` or `io.LimitReader` to protect your server.

---

### Today's Micro-Capstone

**Objective:**
Build a command-dispatch TCP server that uses newline-delimited framing and handles multi-command streams reliably.

**Requirements:**

1. Listen on `127.0.0.1:9999`.
2. Wrap each connection in a buffered reader/scanner to extract lines ending in `\n`.
3. Support three discrete line commands:

- `UPPER <text>` $\rightarrow$ responds with the uppercase version of `<text>` + `\n`.
- `REVERSE <text>` $\rightarrow$ responds with `<text>` reversed + `\n`.
- `QUIT` $\rightarrow$ writes `BYE\n` and closes the connection immediately.
- Any other input responds with `ERR unknown command\n`.

4. **Verification via Netcat / Raw Piping:**
   Test multi-command framing by piping multiple commands simultaneously in one write:

```bash
printf "UPPER hello\nREVERSE golang\nUPPER rock\nQUIT\n" | nc 127.0.0.1 9999

```

Verify that the server executes all four commands sequentially and returns:

```text
HELLO
gnalog
ROCK
BYE

```
