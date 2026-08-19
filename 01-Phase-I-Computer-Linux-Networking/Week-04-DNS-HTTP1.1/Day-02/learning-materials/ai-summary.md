# Week 4 - Day 2 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of DNS Inspection with `dig`, Time-To-Live (TTL) mechanics, Cascading DNS Caching Layers (Browser ➔ OS Stub Resolver ➔ Recursive DNS ➔ Authoritative NS), and Full Referral Tracing (`dig +trace`).

---

## 🌐 Cascading Multi-Tier DNS Caching Pipeline

```text
[ USER REQUEST ]
       │
       ▼
1. Browser DNS Cache ─────── (HIT) ──► Immediate Resolution (0 ms)
       │ (MISS)
       ▼
2. OS Stub Resolver ──────── (HIT) ──► Kernel Resolution (systemd-resolved)
       │ (MISS)
       ▼
3. Recursive Resolver ────── (HIT) ──► Public DNS Cache (8.8.8.8 / 1.1.1.1) [TTL counts down]
       │ (MISS - TTL Expired)
       ▼
4. Root / TLD / Auth NS ──── (Query) ─► Returns fresh authoritative record + initial TTL
```

---

## 1. Practical `dig` Command Reference
- **Basic Query:** `dig example.com A` (Queries default system resolver for A records).
- **Target Specific Resolver:** `dig @8.8.8.8 example.com` (Directs query to Google Public DNS).
- **Full Referral Traversal:** `dig +trace example.com` (Emulates a recursive resolver by walking from `.` root to TLD to Authoritative NS).
- **Short Answer Only:** `dig +short example.com` (Returns only the IP address).

---

## 2. TTL (Time-To-Live) Mechanics
- **Countdown Behavior:** When an authoritative server issues a record with e.g. `TTL = 3600` (1 hour), recursive resolvers cache it and decrement the TTL integer with every passing second.
- **Cache Invalidation Trade-offs:**
  - **High TTL (e.g. 86400s / 24h):** Minimizes DNS lookup latency and reduces authoritative server load; delays IP migration propagation.
  - **Low TTL (e.g. 60s):** Enables rapid IP failover and zero-downtime server migration; increases DNS query volume.
