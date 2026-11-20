### Cold-Start Warmup (60-Second Check)

In Week 9 Day 1, you wrote a TCP server using `net.Listen("tcp", ":8080")` and an infinite `for` loop calling `ln.Accept()`.
When `ln.Accept()` succeeds, it gives you a `conn` (`net.Conn`). **Physically, what does `conn` represent inside the Linux kernel, and how did you send text back to the client through it?**

---

### Week 10, Day 1: The HTTP Handler (`net/http`)

#### 1. The Friction (Why Raw TCP Gets Painful)

In Week 9, when a client connected to your TCP server, you received raw bytes like `GET /hello HTTP/1.1\r\nHost: localhost\r\n\r\n`. If you used raw `net.Listen`, _you_ would have to manually scan for `\r\n`, split the headers, check `Content-Length`, and format `HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n` by hand on every single connection. That is tedious, error-prone plumbing.

#### 2. The Physical Analogy

Think of `net/http` as a **post office sorting machine** attached directly to your TCP pipe. Raw bytes flow in from the street; the machine slices open the envelope, reads the return address and letter contents for you, places them neatly on a tray (`*http.Request`), and hands you a blank stamped reply card (`http.ResponseWriter`) to write your answer on.

#### 3. The Machine Mechanics (Line by Line)

Let us look at the smallest possible HTTP server in Go (only 14 lines) and dissect **why every single word and symbol exists**:

```go
package main

import (
	"fmt"
	"net/http"
)

func helloHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "Shop is open")
}

func main() {
	http.HandleFunc("/hello", helloHandler)
	http.ListenAndServe(":8080", nil)
}

```

Here is why every single piece exists and why it is shaped that way:

1. **`r *http.Request` (Why the `*` pointer?):**
   When the sorting machine parses the incoming HTTP envelope (the URL, headers, method, and body stream), it builds a big `http.Request` struct in heap memory. Instead of copying that whole heavy struct every time it calls your function, it passes a **memory address pointer (`*`)** pointing to where that struct sits in RAM. You read from it (`r.Method`, `r.URL.Path`), and if you modify anything on `r`, you are modifying the real request in memory.
2. **`w http.ResponseWriter` (Why NO `*` pointer here?):**
   Look closely: `r` has a `*`, while `w` does **not**. Why? Because `http.Request` is a concrete **struct** (data sitting in RAM), whereas `http.ResponseWriter` is an **interface** (a contract of behaviors: `Header()`, `Write([]byte)`, `WriteHeader(int)`). In Go, an interface value already holds an internal pointer to the underlying connection object (`*http.response`) inside the standard library. Putting `*http.ResponseWriter` would be a pointer to an interface, which Go forbids because the interface already points to the real socket writer.
3. **`fmt.Fprintln(w, "Shop is open")` (Why `Fprintln` instead of `Println`?):**
   `fmt.Println` is hardcoded to write bytes to file descriptor `1` (`STDOUT` — your terminal screen). `fmt.Fprintln` (File/Writer Println) takes **any `io.Writer**`as its first argument. Because`http.ResponseWriter`implements`Write([]byte) (int, error)`, it satisfies the `io.Writer`interface from Week 8 Day 1. So`fmt.Fprintln(w, ...)` writes your string directly into the HTTP response buffer, which Go flushes down the TCP socket to the user's browser.

4. **`http.HandleFunc("/hello", helloHandler)` (Why pass the function name without `()`?):**
   Notice there are **no parentheses** after `helloHandler`. If you wrote `helloHandler()`, Go would try to _execute_ the function immediately at startup. Without `()`, you are passing the **memory address of the function's machine code** into Go's default routing table (`DefaultServeMux`), telling the sorting machine: _"Whenever a request arrives for `/hello`, jump to this function and run it."_
5. **`http.ListenAndServe(":8080", nil)` (Why `nil` as the second argument?):**
   The first argument `":8080"` tells the OS kernel to bind a TCP listening socket on port 8080 across all network interfaces. The second argument asks: _"Which router (`http.Handler`) should inspect incoming requests?"_ Passing `nil` tells Go: _"Use the global `DefaultServeMux` where I just registered `/hello`."_

#### Failure-Mode Callout

**The #1 Mistake on Day 1:** Calling `http.HandleFunc` _after_ `http.ListenAndServe`. `ListenAndServe` runs an infinite blocking `for { ln.Accept() }` loop—it never returns unless the server crashes or fails to bind the port. Any line written after `ListenAndServe` is dead code that will never execute.

---

### Today's Micro-Capstone (Week 10, Day 1)

Keep it strictly to today's 5 concepts—no JSON, no external packages, no concurrency locks yet:

- **Objective:** Create `day1_http.go` with two routes (`/status` and `/owner`) using only `net/http` and `fmt`, proving you control how `http.ResponseWriter` and `*http.Request` behave.
- **Acceptance Criteria:**

1. Run `go run day1_http.go` and hit `curl -i [http://127.0.0.1:8080/status](http://127.0.0.1:8080/status)` — verify you see `HTTP/1.1 200 OK`, automatic `Date` and `Content-Length` headers added by Go's sorting machine, and your body text.
2. Hit a route you did _not_ register (`curl -i [http://127.0.0.1:8080/unknown](http://127.0.0.1:8080/unknown)`) and observe what status code `DefaultServeMux` automatically sends back.

- **Edge Case to Handle:** Inside your `/status` handler, print `r.Method` and `r.RemoteAddr` into the response body so you can see the exact client IP and ephemeral source port the kernel assigned to your `curl` command.
