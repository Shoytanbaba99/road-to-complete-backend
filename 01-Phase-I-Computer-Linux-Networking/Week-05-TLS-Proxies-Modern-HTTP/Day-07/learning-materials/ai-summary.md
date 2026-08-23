# Week 5 - Day 7 Master Synthesis & Architecture Reference

> **Scope:** Master synthesis of Week 5: TLS 1.3, PKI Certificate Chains, Edge Proxies, TLS Termination, Layer 4 vs Layer 7 Boundaries, and End-to-End Traffic Traversal ([`full_pipeline.py`](learning-materials/full_pipeline.py)).

---

## 🌐 End-to-End Browser ➔ CDN/Edge Proxy ➔ Origin Server Pipeline

```text
[ BROWSER / CLIENT ]
   │
   ├── 1. Cascading DNS Resolution (Host ➔ Edge Proxy IP)
   ├── 2. TCP 3-Way Handshake (SYN ➔ SYN-ACK ➔ ACK) over Layer 4
   ├── 3. TLS 1.3 Handshake (ClientHello + SNI + ECDHE Key Share)
   │      └── Validates Leaf ➔ Intermediate ➔ OS Root CA Trust Chain
   │      └── Derives 256-bit Symmetric Session Key (AES-GCM)
   │
   ▼ ENCRYPTED TLS TRAFFIC OVER PUBLIC INTERNET (Port 443) ▼
┌────────────────────────────────────────────────────────┐
│ GEOGRAPHICALLY LOCAL EDGE CDN / REVERSE PROXY          │
│ - Terminates Client TLS Connection                     │
│ - Offloads CPU Cryptography                            │
│ - Injects Headers: X-Forwarded-For, X-Forwarded-Proto  │
└────────────────────────────────────────────────────────┘
   │
   ▼ UNENCRYPTED HTTP / PRIVATE BACKEND TUNNEL (Port 80) ▼
┌────────────────────────────────────────────────────────┐
│ PRIVATE BACKEND ORIGIN APP SERVER                      │
│ - Reads Client IP from X-Forwarded-For Header          │
│ - Executes Application Logic / Queries Database         │
│ - Returns HTTP 200 OK Response Stream                  │
└────────────────────────────────────────────────────────┘
```

---

## 1. Layer 4 vs. Layer 7 Proxy Comparison

| Dimension | Layer 4 Proxy (TCP/UDP Load Balancer) | Layer 7 Proxy (Reverse Proxy / HTTP Gateway) |
|---|---|---|
| **Protocol Level** | Operates at TCP/IP Socket level (IP & Port) | Operates at HTTP/1.1, HTTP/2, HTTP/3 level |
| **TLS Handling** | Passes encrypted TLS bytes directly to backend (TLS Passthrough) | Terminates TLS, decrypts payload, and inspects HTTP headers (TLS Termination) |
| **Inspection** | Cannot read HTTP headers, paths, cookies, or body | Full access to URL paths, cookies, headers (`X-Forwarded-For`), and JSON body |
| **Performance** | Extremely fast, minimal CPU overhead | Higher CPU overhead (header parsing, routing rules) |
