# Week 3 - Day 3 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Network Layer Routing Tables, Network Address Translation (NAT / SNAT / DNAT), Ports, BSD Socket File Descriptors, Client-Server Architecture, and Multi-Port TCP Knocker (`knocker.c`).

---

## 🌐 End-to-End Packet Traversal & Socket Demultiplexing

```text
[ SOURCE HOST (Client) ]
  └── Process creates Socket (FD 3) ──► bound to ephemeral port (e.g. 52410)
      │
      ▼
[ ROUTERS & NAT (Network Layer) ]
  ├── Gateway rewrites Source IP:Port via SNAT (Private ➔ Public IP)
  ├── Encapsulates IP packet into new Ethernet frames across router hops
  └── Destination Gateway forwards packet to target destination via DNAT
      │
      ▼
[ DESTINATION HOST (Server) ]
  └── Kernel receives TCP segment on Port 5000
      │ (Kernel inspects 4-tuple: Src IP, Src Port, Dst IP, Dst Port)
      ▼
  └── Demultiplexes payload directly into Server's Socket File Descriptor (FD 4)!
```

---

## 1. Network Address Translation (NAT)
- **SNAT (Source NAT):** Translates private RFC 1918 IPs (`192.168.x.x`) into a single public WAN IP address when outbound packets leave the home/office router.
- **DNAT (Destination NAT / Port Forwarding):** Translates incoming public traffic to specific internal private IP addresses.

---

## 2. Ports & Socket File Descriptors
- **Ports (16-bit, 0–65535):** Allow the OS to route network data to specific processes.
- **Sockets:** Special file descriptors returned by `socket(AF_INET, SOCK_STREAM, 0)`.
- **4-Tuple Connection Identifier:** `(Source IP, Source Port, Destination IP, Destination Port)` uniquely identifies every active network stream in the kernel.

---

## 3. TCP Port Knocker Capstone ([`knocker.c`](learning-materials/knocker.c))
- Takes Target IP and array of Ports.
- Uses `inet_pton(AF_INET, ip, &in_addr)` for IP conversion.
- Creates socket, populates `struct sockaddr_in` with `htons(port)`, calls `connect()`, transmits `"Knock Knock!"` on success, and handles connection refusals with `strerror(errno)`.
