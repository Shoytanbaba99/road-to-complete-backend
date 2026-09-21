# Week 9 - Day 1: TCP with Go — `net.Listen`, Sockets & the Accept Loop

---

## 📋 Objectives
- [x] Understand OS-level socket creation, IP/port binding, and listening states
- [x] Implement passive TCP listening using `net.Listen("tcp", addr)`
- [x] Write an idiomatic blocking accept loop with `listener.Accept()`
- [x] Read from and write to incoming peer connections using `net.Conn`
- [x] Handle connection teardown and graceful `io.EOF` termination
- [x] Build the hands-on **TCP Echo & Banner Server Lab (`go lab`)**

---

## 🗺️ Day 1 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | Personal mental model on kernel/NIC socket layers, `net.Listen()`, and `listener.Accept()`. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming kernel accept queues, TCP 3-way handshake completion, and `net.Conn` stream lifecycles. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Textbook reference documentation on Go `net` package and TCP sockets. |
| 🛠️ [**`learning-materials/go lab/main.go`**](learning-materials/go%20lab/main.go) | **Hands-On Server:** TCP listener on `127.0.0.1:8888`, accept loop, welcome banner emission, and continuous byte read loop with `io.EOF` handling. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[TCP Sockets in Go — net.Listen, Kernel Queues & the Accept Loop]]` in `Engineers-Playbook/02 Permanent/`
