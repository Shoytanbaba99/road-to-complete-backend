# Week 3 - Day 6: TCP Flow Control, Congestion Control, Teardown & TIME_WAIT

---

## 📋 Objectives
- [x] TCP Flow Control (`rwnd`, socket buffer backpressure, Zero Window probes)
- [x] TCP Congestion Control (`cwnd`, Slow Start, Congestion Avoidance, AIMD)
- [x] Connection Teardown 4-Way Handshake (`FIN` ➔ `ACK` ➔ `FIN` ➔ `ACK`)
- [x] `TIME_WAIT` state mechanics ($2 \times MSL$ timer)
- [x] TCP Keep-Alive probes & idle connection detection
- [x] Build C flow-control backpressure lab ([`server.c`](learning-materials/server.c) & [`client.c`](learning-materials/client.c))

---

## 🗺️ Day 6 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of `rwnd` flow control, `cwnd` AIMD congestion control, and `TIME_WAIT`. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, RFC 793 / RFC 5681 specifications, and state machine transition rules. |
| 🛠️ [**`learning-materials/server.c`**](learning-materials/server.c) | C TCP Server setting tiny `SO_RCVBUF` to demonstrate zero-window flow control backpressure. |
| 🛠️ [**`learning-materials/client.c`**](learning-materials/client.c) | C TCP Client sending continuous 512-byte payload chunks until socket buffer blocks `send()`. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile flow control server and client
gcc server.c -o server
gcc client.c -o client

# Run server in background
./server &

# Run client to trigger buffer backpressure
./client
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[TCP Flow Control, Congestion Control & TIME_WAIT Mechanics]]` in `Engineers-Playbook/02 Permanent/`
