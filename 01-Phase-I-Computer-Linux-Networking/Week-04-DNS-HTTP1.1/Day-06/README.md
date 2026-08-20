# Week 4 - Day 6: HTTP Debugging, Persistent Connections & Packet Capturing

---

## 📋 Objectives
- [x] HTTP/1.1 persistent connections (`Connection: keep-alive`) & TCP reuse
- [x] Application protocol inspection using `curl -v`
- [x] Raw HTTP transaction crafting via `nc` (Netcat)
- [x] Packet capture & flag analysis with `tcpdump` (`S`, `.`, `P`, `F`)
- [x] Compare single-connection reuse vs. multi-connection overhead

---

## 🗺️ Day 6 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and `tcpdump` flag legend. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of HTTP keep-alive, `curl` verbose inspection, and `tcpdump` packet filtering. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, RFC 7230 connection management specs, and `tcpdump` command syntax guide. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Verbose HTTP inspection with curl
curl -v http://example.com

# Raw HTTP transaction with Netcat
nc example.com 80

# Capture TCP packets and ASCII payload on port 80
sudo tcpdump -i any -nn -A port 80
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[HTTP/1.1 Persistent Connections, Netcat Probing & tcpdump Inspection]]` in `Engineers-Playbook/02 Permanent/`
