# Week 4 - Day 3: HTTP/1.1 Wire Format & Raw Socket Request/Response Parsing

---

## 📋 Objectives
- [x] HTTP/1.1 wire protocol structure & RFC 7230 standards
- [x] Request line (Method, Target Path, HTTP Version) & Status line (HTTP Version, Status Code, Reason)
- [x] CRLF (`\r\n`) header delimiters & empty line body separator (`\r\n\r\n`)
- [x] `Content-Length` framing & stream body reading
- [x] Build Raw Socket HTTP/1.1 Client ([`http_client.py`](learning-materials/http_client.py))

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and raw request/response formatting examples. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of HTTP wire format, CRLF framing, and stream body parsing rules. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, RFC 7230 HTTP specs, and header field grammar rules. |
| 🛠️ [**`learning-materials/http_client.py`**](learning-materials/http_client.py) | **Capstone:** Raw socket HTTP/1.1 client parsing HTTP status line, headers, and Content-Length body payload. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Run raw socket HTTP client against postman-echo.com
python3 http_client.py
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[HTTP/1.1 Wire Format, Request-Response Anatomy & Raw Socket Parsing]]` in `Engineers-Playbook/02 Permanent/`
