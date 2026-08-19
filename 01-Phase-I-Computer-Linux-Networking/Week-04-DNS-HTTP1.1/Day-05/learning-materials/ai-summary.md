# Week 4 - Day 5 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of HTTP Status Code Semantics (1xx, 2xx, 3xx, 4xx, 5xx), Cookie Management (`Set-Cookie` / `Cookie` flags), HTTP Caching Control (`Cache-Control`, `max-age`, `no-cache`), and ETag Conditional Requests (`If-None-Match` ➔ `304 Not Modified`, `If-Match` ➔ `412 Precondition Failed`).

---

## 🌐 HTTP Status Codes & Conditional Request Protocol

```text
[ CONDITIONAL GET CACHE VALIDATION (If-None-Match) ]
  Client                                           Server
    │── GET /avatar.png ────────────────────────────►│ Generates ETag: "v123"
    │◄── 200 OK (ETag: "v123", Body: 500KB) ────────┤ Stores image in local cache
    │
    │── GET /avatar.png (If-None-Match: "v123") ────►│ Checks ETag "v123" against current
    │◄── 304 Not Modified (EMPTY BODY! 0 KB) ───────┤ (Server confirms cache is valid!)

[ OPTIMISTIC CONCURRENCY CONTROL (If-Match) ]
  Client A                                         Server (Current ETag: "v1")
    │── PUT /item/1 (If-Match: "v1", Body) ─────────►│ Matches "v1"! Updates item to "v2".
    │◄── 200 OK (ETag: "v2") ───────────────────────┤
    │
  Client B (Stale ETag: "v1")                      Server (Current ETag: "v2")
    │── PUT /item/1 (If-Match: "v1", Body) ─────────►│ "v1" != "v2"! Conflict detected!
    │◄── 412 Precondition Failed ───────────────────┤ Rejects mid-air collision!
```

---

## 1. HTTP Status Code Taxonomy
- **`1xx` Informational:** Protocol switching or processing (`101 Switching Protocols`).
- **`2xx` Success:** Action received and understood (`200 OK`, `201 Created`, `204 No Content`).
- **`3xx` Redirection:** Further action required (`301 Moved Permanently`, `302 Found`, `304 Not Modified`).
- **`4xx` Client Error:** Bad request syntax or unauthorized (`400 Bad Request`, `401 Unauthorized`, `403 Forbidden`, `404 Not Found`, `412 Precondition Failed`).
- **`5xx` Server Error:** Server failed to fulfill valid request (`500 Internal Server Error`, `502 Bad Gateway`, `503 Service Unavailable`, `504 Gateway Timeout`).

---

## 2. Cookie Security Attributes
- **`HttpOnly`:** Prevents client-side JavaScript (`document.cookie`) from accessing the cookie (mitigates XSS token theft).
- **`Secure`:** Ensures browser only sends the cookie over encrypted HTTPS connections.
- **`SameSite`:** Controls cross-site cookie transmission (`Strict`, `Lax`, `None`) to prevent CSRF attacks.
