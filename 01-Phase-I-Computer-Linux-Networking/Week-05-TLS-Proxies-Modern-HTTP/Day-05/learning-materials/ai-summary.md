# Week 5 - Day 5 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Forward Proxy vs. Reverse Proxy, Load Balancers, TLS Termination, Forwarded Headers (`X-Forwarded-For`, `X-Forwarded-Proto`), and System Cluster Architecture ([`system_cluster.py`](learning-materials/system_cluster.py)).

---

## 🌐 Forward Proxy vs. Reverse Proxy vs. Load Balancer

```text
[ FORWARD PROXY (Acts on behalf of CLIENTS) ]
  Client A ──┐
  Client B ──┼──► Forward Proxy (Hides Client IPs, Outbound Filtering) ──► Public Web
  Client C ──┘

[ REVERSE PROXY & LOAD BALANCER with TLS TERMINATION (Acts on behalf of SERVERS) ]
  Public Web / Clients
          │ (Encrypted HTTPS on Port 443)
          ▼
  ┌────────────────────────────────────────────────────────┐
  │ REVERSE PROXY / LOAD BALANCER (Nginx / HAProxy / ALB)   │
  │ - Decrypts TLS (TLS Termination)                       │
  │ - Injects Headers (X-Forwarded-For: ClientIP)          │
  └────────────────────────────────────────────────────────┘
          │ (Plain HTTP over Internal Private Network)
     ┌────┴───────────────┬───────────────────┐
     ▼                    ▼                   ▼
  Backend App 1        Backend App 2       Backend App 3
```

---

## 1. Key Architectural Concepts
- **Reverse Proxy:** Gateway exposing a single public endpoint to external clients while forwarding requests to multiple internal backend services.
- **Load Balancer:** Distributes incoming traffic across private backend instances (Round-Robin, Least Connections) to prevent overload and ensure high availability.
- **TLS Termination:** Decrypts HTTPS at the Reverse Proxy boundary and routes unencrypted HTTP requests over private internal networks to application instances.
- **Forwarded Headers:** Proxy appends `X-Forwarded-For` (client IP) and `X-Forwarded-Proto` (`https`) so downstream app servers know the original client's IP and connection protocol.
