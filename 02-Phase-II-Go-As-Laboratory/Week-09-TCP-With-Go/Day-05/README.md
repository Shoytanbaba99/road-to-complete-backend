# Week 9 - Day 5: TCP with Go — Build a Production Echo Server

---

## 📋 Objectives
- [x] Build an end-to-end robust, concurrent TCP echo server with stdlib `net`
- [x] Implement asymmetric timeouts: rolling read deadline (`SetReadDeadline`) and scoped write deadline (`SetWriteDeadline`)
- [x] Protect server memory from malicious/oversized payloads using `scanner.Buffer(nil, MaxlineLength)`
- [x] Catch and handle `bufio.ErrTooLong` explicitly, returning error messages before closing connections
- [x] Build the hands-on **Production Echo Server Lab (`go lab`)**

---

## 🗺️ Day 5 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | Personal synthesis on write deadlines to prevent slow-client backpressure, and `scanner.Buffer` payload ceilings. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Day specifications for building an echo server. |
| 🛠️ [**`learning-materials/go lab/main.go`**](learning-materials/go%20lab/main.go) | **Hands-On Server:** TCP listener on `:8080`, concurrent connection handling, 64KB line length limit, 10s read timeout, 5s write timeout, and `bufio.ErrTooLong` handling. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[TCP Echo Servers — Write Deadlines, Backpressure & Buffer Capping]]` in `Engineers-Playbook/02 Permanent/`
