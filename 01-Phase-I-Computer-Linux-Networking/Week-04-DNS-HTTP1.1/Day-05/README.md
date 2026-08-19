# Week 4 - Day 5: HTTP Status Codes, Cookies, Caching & Conditional ETags

---

## 📋 Objectives
- [x] HTTP Status code class semantics (`1xx`, `2xx`, `3xx`, `4xx`, `5xx`)
- [x] Cookie state management (`Set-Cookie`, `Cookie`, `HttpOnly`, `Secure`, `SameSite`)
- [x] Cache validation headers (`Cache-Control`, `max-age`, `no-cache`, `no-store`)
- [x] ETag conditional requests (`If-None-Match` ➔ `304 Not Modified`, `If-Match` ➔ `412 Precondition Failed`)
- [x] Study stateful session & conditional server ([`state_server.py`](learning-materials/state_server.py))

---

## 🗺️ Day 5 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and refined ETag conditional request rules. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of status code classes, cookie security flags, and ETag concurrency control. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, RFC 7232 conditional request specs, and RFC 6265 cookie standards. |
| 🛠️ [**`learning-materials/state_server.py`**](learning-materials/state_server.py) | **Capstone:** State & caching server issuing `Set-Cookie` headers, validating `If-None-Match`, and returning `304 Not Modified`. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Start Stateful & Conditional HTTP Server
python3 state_server.py &

# Send initial request to receive Set-Cookie and ETag headers
curl -i http://127.0.0.1:8082/profile

# Send conditional GET with If-None-Match to receive 304 Not Modified
curl -i -H 'If-None-Match: "v1.0"' http://127.0.0.1:8082/profile
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[HTTP Status Codes, Cookie Security & Conditional ETag Caching]]` in `Engineers-Playbook/02 Permanent/`
