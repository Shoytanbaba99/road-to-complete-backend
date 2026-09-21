# Week 9 - Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of TCP Socket Programming in Go: Kernel Socket Layer, `net.Listen`, Socket Binding & Listening Queues (SYN/Accept queues), The Accept Loop (`listener.Accept()`), and Stream I/O via `net.Conn`.

---

## 🌐 TCP Socket Lifecycle Architecture

```text
               User Space (Go)                         Kernel Space / OS Sockets
               ────────────────                        ─────────────────────────
             net.Listen("tcp", addr)  ────────►  socket() + bind() + listen()
                                                           │
                                                           ▼
                                                    [SYN Backlog Queue]
                                                           │ (3-Way Handshake SYN-ACK)
                                                           ▼
                                                   [Accept Backlog Queue]
                                                           │
             for {                                         │
                 conn, err := listener.Accept() ◄──────────┘ (sys_accept4 / accept)
                 │
                 ▼
                 handleConnection(conn)
                 ├── conn.Write([]byte(banner)) ────────► TCP TX Buffer ──► NIC
                 ├── conn.Read(buf)             ◄──────── TCP RX Buffer ◄── NIC
                 └── defer conn.Close()         ────────► FIN Handshake (Teardown)
             }
```

---

## 1. Core Mechanics & Engineering Principles

| Go / OS Abstraction | Underlying Syscall / Structure | Engineering Function |
|---|---|---|
| **`net.Listen("tcp", addr)`** | `socket(AF_INET, SOCK_STREAM)` + `bind()` + `listen()` | Creates a passive listening socket descriptor and initializes the kernel connection queues. |
| **`listener.Accept()`** | `accept()` / `accept4()` | Blocks until a connection completes the TCP 3-way handshake in the kernel accept queue, then returns a dedicated connected socket descriptor. |
| **`net.Conn`** | Stream Socket File Descriptor | Implements `io.Reader`, `io.Writer`, and `io.Closer` for bidirectional byte communication with a remote peer. |
| **`conn.RemoteAddr()` / `LocalAddr()`** | `getpeername()` / `getsockname()` | Retrieves IP and ephemeral port metadata for the connected socket pair. |
| **`io.EOF` Handling** | Read returns `0, io.EOF` | Indicates the remote peer sent a `FIN` packet to gracefully close their write half of the connection. |
