# Week 5 - Day 6: Modern HTTP — HTTP/2 Multiplexing & HTTP/3 / QUIC Protocol

---

## 📋 Objectives
- [x] HTTP/2 Binary Framing Layer & Stream Multiplexing (Odd vs Even stream IDs)
- [x] HPACK Header Compression (Static & Dynamic indexing tables)
- [x] HTTP/2 Server Push mechanics
- [x] HTTP/3 QUIC protocol over UDP (Eliminating TCP Head-of-Line Blocking)
- [x] QUIC 0-RTT/1-RTT TLS 1.3 handshake & Connection ID migration

---

## 🗺️ Day 6 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal master mental model synthesizing HTTP/2 framing, HPACK tables, and HTTP/3 QUIC UDP architecture. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference comparing HTTP/1.1 vs HTTP/2 vs HTTP/3 specifications. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory on RFC 7540 HTTP/2, RFC 7541 HPACK, and RFC 9000 QUIC. |
| 🛠️ [**`learning-materials/h2_framer.py`**](learning-materials/h2_framer.py) | **Reference Script:** Python HTTP/2 binary frame parsing and stream header decoder simulation. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[HTTP/2 Binary Framing, HPACK & HTTP/3 QUIC UDP Protocol]]` in `Engineers-Playbook/02 Permanent/`
