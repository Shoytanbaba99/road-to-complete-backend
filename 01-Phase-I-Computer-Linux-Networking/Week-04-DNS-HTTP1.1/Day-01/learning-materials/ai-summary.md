# Week 4 - Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Domain Name System (DNS), Hierarchical Namespace, Recursive Resolvers (`1.1.1.1`, `8.8.8.8`), Root Servers (`.`), TLD Nameservers, Authoritative Nameservers, Zone files, and Resource Record Types (`A`, `AAAA`, `CNAME`, `TXT`, `NS`, `MX`).

---

## 🌐 Recursive DNS Resolution Traversal

```text
[ CLIENT ] ── (1. Query: api.example.com) ──► [ RECURSIVE RESOLVER (1.1.1.1 / 8.8.8.8) ]
                                                        │
    ┌───────────────────────────────────────────────────┤
    │ (2. Query: api.example.com?)                      │ (4. Query: api.example.com?)
    ▼                                                   ▼
[ ROOT SERVER (.) ]                                [ TLD SERVER (.com) ]
    │ (3. Refer: Go to .com TLD)                        │ (5. Refer: Go to ns1.cloudflare.com)
    └───────────────────────────────────────────────────┤
                                                        │
                                                        │ (6. Query: api.example.com?)
                                                        ▼
                                           [ AUTHORITATIVE NAMESERVER ]
                                                        │ (7. Answer: A -> 104.20.23.154)
[ CLIENT ] ◄── (8. Return IP: 104.20.23.154) ───────────┘
```

---

## 1. DNS Record Types Cheat Sheet

| Record Type | Full Name | Purpose | Example |
|---|---|---|---|
| **`A`** | IPv4 Address | Maps hostname to 32-bit IPv4 address | `example.com ➔ 93.184.216.34` |
| **`AAAA`** | IPv6 Address | Maps hostname to 128-bit IPv6 address | `example.com ➔ 2606:2800:220:1:248:1893:25c8:1946` |
| **`CNAME`** | Canonical Name | Aliases one hostname to another (Canonical domain) | `www.example.com ➔ example.com` |
| **`MX`** | Mail Exchanger | Directs email to domain mail servers with priority | `example.com ➔ 10 mail.example.com` |
| **`NS`** | Name Server | Delegates a DNS zone to authoritative servers | `example.com ➔ ns1.cloudflare.com` |
| **`TXT`** | Text Record | Carries arbitrary text (SPF, DKIM, verification tokens) | `example.com ➔ "v=spf1 include:_spf.google.com ~all"` |

---

## 2. Key Architecture Principles
- **Root Zone (`.`):** Served by 13 IP root server clusters distributed globally via Anycast BGP.
- **Apex CNAME Rule (RFC 1912):** A `CNAME` record cannot coexist with any other record type (`SOA`, `NS`, `MX`) at the zone apex (`example.com`), which is why apex aliases traditionally require `ALIAS` or `ANAME` provider extensions.
