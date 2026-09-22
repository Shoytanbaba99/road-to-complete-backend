# Week 9 - Day 4: TCP with Go — Timeouts (`SetDeadline`) & Connection Shutdown

---

## 📋 Objectives
- [x] Protect TCP servers from idle connection leaks, resource exhaustion, and Slowloris attacks
- [x] Master socket deadline mechanics: `conn.SetDeadline`, `conn.SetReadDeadline`, and `conn.SetWriteDeadline`
- [x] Discriminate timeout errors from network faults using `errors.As(err, &netErr)` and `netErr.Timeout()`
- [x] Understand graceful TCP connection teardown vs. half-close (`tcpConn.CloseWrite()`)
- [x] Build the hands-on **Timeout-Protected Server with Half-Close Support Lab (`go lab`)**

---

## 🗺️ Day 4 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | Personal synthesis of socket deadlines, idle timeouts, and separating read/write deadlines. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | In-depth theory covering socket deadlines, `net.Error`, TCP FIN/RST packet exchange, and `CloseWrite()`. |
| 🛠️ [**`learning-materials/go lab/main.go`**](learning-materials/go%20lab/main.go) | **Hands-On Server:** TCP listener on `127.0.0.1:9002` with 5s idle timeout (`SetReadDeadline`), `netErr.Timeout()` inspection, and half-close via `(*net.TCPConn).CloseWrite()`. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[TCP Timeouts & Socket Shutdown — Deadlines, net.Error & CloseWrite]]` in `Engineers-Playbook/02 Permanent/`
