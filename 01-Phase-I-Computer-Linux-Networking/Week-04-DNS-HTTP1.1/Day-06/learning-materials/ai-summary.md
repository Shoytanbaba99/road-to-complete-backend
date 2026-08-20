# Week 4 - Day 6 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of HTTP/1.1 Persistent Connections (`Connection: keep-alive`), Terminal Protocol Debugging with `curl -v`, Raw Sockets with `nc`, and Network Packet Inspection with `tcpdump`.

---

## 🌐 Persistent HTTP/1.1 Connection vs Non-Persistent HTTP/1.0

```text
[ NON-PERSISTENT HTTP/1.0 (Connection Per Request) ]
  Client                                           Server
    ├── SYN ➔ SYN-ACK ➔ ACK (1 RTT) ──────────────►│ (TCP Established)
    ├── GET /index.html ──────────────────────────►│
    │◄── 200 OK (Payload) ────────────────────────┤ (TCP Close / FIN)
    │
    ├── SYN ➔ SYN-ACK ➔ ACK (1 RTT) ──────────────►│ (NEW TCP Established!)
    ├── GET /style.css ───────────────────────────►│
    │◄── 200 OK (Payload) ────────────────────────┤ (TCP Close / FIN)

[ PERSISTENT HTTP/1.1 (Connection Reuse) ]
  Client                                           Server
    ├── SYN ➔ SYN-ACK ➔ ACK (1 RTT) ──────────────►│ (TCP Established ONCE)
    ├── GET /index.html ──────────────────────────►│
    │◄── 200 OK (Payload, Connection: keep-alive) ─┤
    │
    ├── GET /style.css (REUSES EXISTING SOCKET!) ─►│ (0 Connection RTT Overhead!)
    │◄── 200 OK (Payload) ────────────────────────┤
```

---

## 1. Terminal Inspection Tooling Reference
- **`curl -v http://example.com`:** Displays DNS resolution, TCP connection establishment, outbound HTTP request headers, and incoming response headers.
- **`nc example.com 80`:** Opens a raw TCP socket. Type `GET / HTTP/1.1\r\nHost: example.com\r\n\r\n` to manually drive the protocol.
- **`sudo tcpdump -i any -nn -A port 80`:** Captures raw Ethernet packets on port 80, printing ASCII HTTP payloads and TCP flags (`S`, `.`, `P`, `F`).

---

## 2. `tcpdump` Flag Legend
- `S` (SYN): Connection Synchronization request.
- `.` (ACK): Acknowledgement.
- `P` (PUSH): PSH flag set; instructs OS buffer to flush data immediately to application.
- `F` (FIN): Connection Termination request.
- `R` (RST): Connection Reset / abrupt termination.
