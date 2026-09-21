# Week 9 - Day 3: TCP with Go — Concurrent Client Handling & Goroutines

---

## 📋 Objectives
- [x] Understand Go's concurrency model: Concurrency (composition) vs. Parallelism (simultaneous execution)
- [x] Master user-level green threads (Goroutines) and Go's M:N GMP runtime scheduler (Goroutines, OS Threads, Logical Processors)
- [x] Spawn asynchronous worker goroutines per connection (`go handleConnection(conn)`)
- [x] Observe runtime metrics and goroutine lifecycles via `runtime.NumGoroutine()`
- [x] Avoid server deadlocks by preventing blocking I/O on the primary accept loop
- [x] Build the hands-on **Concurrent Echo Server with Diagnostic Metrics Lab (`go lab`)**

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | Personal synthesis of goroutines as lightweight user threads, concurrency vs. parallelism, and the M:N GMP scheduler model. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Deep theory on Go runtime scheduler internals, thread stacks vs. goroutine stacks, and connection multiplexing. |
| 🛠️ [**`learning-materials/go lab/main.go`**](learning-materials/go%20lab/main.go) | **Concurrent TCP Server:** Non-blocking accept loop dispatching `go handleConnection(conn)`, background diagnostic heartbeat, and connection count tracking with `runtime.NumGoroutine()`. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[TCP Concurrency in Go — Goroutines, GMP Scheduler & Connection Dispatch]]` in `Engineers-Playbook/02 Permanent/`
