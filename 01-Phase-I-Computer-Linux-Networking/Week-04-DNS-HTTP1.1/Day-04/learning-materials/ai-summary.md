# Week 4 - Day 4 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of HTTP Methods (`GET`, `POST`, `PUT`, `PATCH`, `DELETE`, `HEAD`, `OPTIONS`), Method Semantics (Safe vs. Idempotent), Content Negotiation (`Accept`, `Accept-Language`, $q$-factor weighting), `Vary` Headers, and REST Server ([`rest_server.py`](learning-materials/rest_server.py)).

---

## 🌐 HTTP Method Semantics Matrix (RFC 7231)

| Method | Safe (Read-Only) | Idempotent | Request Body | Description |
|---|---|---|---|---|
| **`GET`** | ✅ Yes | ✅ Yes | ❌ No | Retrieves resource representation. |
| **`HEAD`** | ✅ Yes | ✅ Yes | ❌ No | Same as GET, but returns ONLY headers (no body). |
| **`OPTIONS`** | ✅ Yes | ✅ Yes | ❌ No | Queries server/resource allowed methods (CORS preflight). |
| **`PUT`** | ❌ No | ✅ Yes | ✅ Yes | Replaces target resource ENTIRELY or creates it at path. |
| **`DELETE`** | ❌ No | ✅ Yes | ❌ Optional | Deletes target resource at URI path. |
| **`POST`** | ❌ No | ❌ No | ✅ Yes | Submits entity for processing (creates new subordinate resource). |
| **`PATCH`** | ❌ No | ❌ No | ✅ Yes | Applies PARTIAL modifications to a resource. |

---

## 1. Content Negotiation & $q$-Factor Algorithm
- **Accept Header:** Clients specify acceptable media types with quality weightings ($q$-values ranging from `0.0` to `1.0`).
- **Parsing Example:** `text/html, application/json;q=0.9, text/plain;q=0.5`
  - Priority 1: `text/html` ($q = 1.0$)
  - Priority 2: `application/json` ($q = 0.9$)
  - Priority 3: `text/plain` ($q = 0.5$)
- **406 Not Acceptable:** Returned if the server cannot serve any representation matching non-zero $q$-factors.

---

## 2. The `Vary` Header
- **HTTP Caching Control:** When a server delivers content tailored via Content Negotiation (`Accept`, `Accept-Encoding`), it MUST include a `Vary: Accept` header.
- **CDN / Proxy Impact:** Tells downstream cache nodes to store separate cache entries per unique `Accept` header value, preventing a cached JSON payload from being served to a browser expecting HTML.
