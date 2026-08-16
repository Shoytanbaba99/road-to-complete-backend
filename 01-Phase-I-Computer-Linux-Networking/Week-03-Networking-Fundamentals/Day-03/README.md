# Week 3 - Day 3: Routing Tables, NAT, Ports, Sockets & Client-Server Architecture

---

## 📋 Objectives
- [x] Kernel routing tables & router frame encapsulation/decapsulation
- [x] Network Address Translation (NAT / SNAT / DNAT)
- [x] 16-bit Port numbers (Demultiplexing data to application processes)
- [x] BSD Sockets as OS File Descriptors (`socket()`, `bind()`, `connect()`)
- [x] 4-Tuple stream identification `(Src IP, Src Port, Dst IP, Dst Port)`
- [x] Build C multi-port TCP knocker capstone ([`knocker.c`](learning-materials/knocker.c))

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of NAT, Ports, Sockets, 4-tuples, and client-server socket APIs. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX socket specifications, and NAT translation tables. |
| 🛠️ [**`learning-materials/knocker.c`**](learning-materials/knocker.c) | **Capstone:** C TCP Port Knocker / Scanner testing target ports via socket `connect()`. |
| 🔬 [**`learning-materials/server.c`**](learning-materials/server.c) | C TCP Server binding to a port and listening for incoming client connections. |
| 🔬 [**`learning-materials/client.c`**](learning-materials/client.c) | C TCP Client establishing a socket connection and sending messages. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile server, client, and port knocker
gcc server.c -o server
gcc client.c -o client
gcc knocker.c -o knocker

# Start server in background on port 5000
./server &

# Run port knocker against ports 4000, 5000, 6000
./knocker 127.0.0.1 4000 5000 6000
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Routing Tables, NAT, Ports, Sockets & Client-Server Architecture]]` in `Engineers-Playbook/02 Permanent/`
