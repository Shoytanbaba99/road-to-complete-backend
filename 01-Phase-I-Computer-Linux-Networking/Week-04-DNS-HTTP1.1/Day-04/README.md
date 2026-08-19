# Week 4 - Day 4: HTTP Methods, Idempotency & Content Negotiation

---

## 📋 Objectives
- [x] HTTP Method semantics (`GET`, `POST`, `PUT`, `PATCH`, `DELETE`, `HEAD`, `OPTIONS`)
- [x] Safe methods (read-only side-effect free) vs. Unsafe methods
- [x] Idempotency mechanics ($N$ identical requests = $1$ request result)
- [x] Content Negotiation (`Accept`, `Content-Type`, $q$-factor weighting)
- [x] `Vary` header caching implications
- [x] Build Content Negotiation & REST Server ([`rest_server.py`](learning-materials/rest_server.py))

---

## 🗺️ Day 4 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and method safety/idempotency rules. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of HTTP verb semantics, $q$-factor parsing, and `Vary` headers. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, RFC 7231 specifications, and REST representation standards. |
| 🛠️ [**`learning-materials/rest_server.py`**](learning-materials/rest_server.py) | **Capstone:** REST server with custom $q$-factor Content Negotiation engine rendering JSON/HTML/Text representations. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Start Content Negotiation REST server
python3 rest_server.py &

# Request JSON representation
curl -i -H "Accept: application/json" http://127.0.0.1:8081/products/1

# Request HTML representation
curl -i -H "Accept: text/html" http://127.0.0.1:8081/products/1

# Test q-factor weighting
curl -i -H "Accept: text/plain;q=0.5, application/json;q=0.9" http://127.0.0.1:8081/products/1
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[HTTP Methods, Idempotency & Content Negotiation Mechanics]]` in `Engineers-Playbook/02 Permanent/`
