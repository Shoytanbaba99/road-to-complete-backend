# Week 4 - Day 7 Review & Capstone Technical Reference

> **Scope:** Full architectural synthesis of Week 4 Application Primitives (DNS Namespace, Recursive Traversal, TTLs, HTTP/1.1 Wire Format, Request-Response Anatomy, Method Safety/Idempotency, Content Negotiation, Cookies, HTTP Caching, and ETag Validation).

---

## 🌐 The Complete Journey: Typing a URL to Receiving Bytes

```text
[ 1. BROWSER URL PARSING ]
  ├── Scheme: http://
  └── Hostname: example.com, Path: /api/v1/data

[ 2. CASCADING DNS RESOLUTION ]
  ├── Check Browser DNS Cache ──► (MISS)
  ├── Check OS Stub Resolver Cache ──► (MISS)
  └── Query Recursive Resolver (1.1.1.1) ──► Root (.) ➔ TLD (.com) ➔ Auth NS ➔ IP: 93.184.216.34

[ 3. TRANSPORT & NETWORK LAYER CONNECTION ]
  ├── Socket Creation: socket(AF_INET, SOCK_STREAM, 0)
  └── TCP 3-Way Handshake: SYN ➔ SYN-ACK ➔ ACK (Port 80)

[ 4. APPLICATION LAYER HTTP/1.1 TRANSACTION ]
  ├── Outbound Wire Format:
  │   GET /api/v1/data HTTP/1.1\r\n
  │   Host: example.com\r\n
  │   Accept: application/json\r\n
  │   If-None-Match: "v1.0"\r\n\r\n
  │
  └── Inbound Response:
      HTTP/1.1 304 Not Modified\r\n
      ETag: "v1.0"\r\n
      Cache-Control: max-age=3600\r\n\r\n
      (Empty Body - Served directly from browser local cache!)
```

---

## 1. Week 4 Deliverable Summary
- **Master Synthesis:** Sealed understanding of how application layer protocols (DNS & HTTP/1.1) sit on top of transport layer sockets (TCP/UDP) and network layer IP routing.
- **DNS vs. HTTP Caching Separation:** Established clear boundary between DNS name resolution caching (governed by DNS TTL) and HTTP entity representation caching (governed by `Cache-Control` and `ETag`).
