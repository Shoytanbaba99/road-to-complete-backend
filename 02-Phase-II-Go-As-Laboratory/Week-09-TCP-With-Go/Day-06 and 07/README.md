# Week 9 - Day 6 & 7: Custom Text Protocol & Packet/Failure Inspection

---

## 📋 Objectives
- [x] Build a tiny custom stateful text protocol (`ADD`, `TAKE`, `CHECK`, `QUIT`)
- [x] Handle connection framing, bounded line buffering (1KB buffer ceiling), and idle read deadlines (30s)
- [x] Inspect connection lifecycle and TCP handshakes using packet/socket tools (`tcpdump`, `ss`)
- [x] Intentionally test boundary conditions and malformed client payloads

---

## 🗺️ Pathways & Files

| File | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | Personal take on socket allocation, buffer scanning, timeouts, and inspecting handshakes via `tcpdump`. |
| 🛠️ [**`learning-materials/go lab/main.go`**](learning-materials/go%20lab/main.go) | Custom stateful inventory protocol server on `:9090` with bounded buffers and read timeouts. |
