# Week 3 - Day 4: User Datagram Protocol (UDP), Connectionless Sockets & RUDP

---

## 📋 Objectives
- [x] User Datagram Protocol (UDP) header & datagram boundaries
- [x] Connectionless communication (`SOCK_DGRAM`, `sendto`, `recvfrom`)
- [x] Unreliable transmission trade-offs (Packet loss, out-of-order delivery, low latency)
- [x] Socket timeouts via `setsockopt(SO_RCVTIMEO)`
- [x] Custom Reliable UDP (RUDP) stop-and-wait ARQ protocol ([`rudp_client.c`](learning-materials/rudp_client.c) & [`rudp_server.c`](learning-materials/rudp_server.c))

---

## 🗺️ Day 4 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of UDP datagrams, `SOCK_DGRAM`, socket timeouts, and RUDP ARQ mechanics. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, RFC 768 UDP specifications, and RUDP stop-and-wait protocol rules. |
| 🛠️ [**`learning-materials/rudp_client.c`**](learning-materials/rudp_client.c) | **Capstone:** RUDP Client implementing Stop-and-Wait ARQ file transfer with timeout retransmissions. |
| 🛠️ [**`learning-materials/rudp_server.c`**](learning-materials/rudp_server.c) | **Capstone:** RUDP Server receiving file chunks, checking sequence numbers, and returning ACKs. |
| 🔬 [**`learning-materials/udp_server.c`**](learning-materials/udp_server.c) | Raw unbuffered UDP ECHO server receiving datagrams on `SOCK_DGRAM`. |
| 🔬 [**`learning-materials/udp_client.c`**](learning-materials/udp_client.c) | Raw unbuffered UDP client sending atomic datagrams. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile RUDP client and server
gcc rudp_server.c -o rudp_server
gcc rudp_client.c -o rudp_client

# Generate a 50 KB dummy binary payload
head -c 51200 /dev/urandom > test_input.bin

# Run RUDP server in background on port 9000
./rudp_server 9000 received_file.txt &

# Send binary file via RUDP client
./rudp_client 127.0.0.1 9000 test_input.bin
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[UDP Datagrams, Connectionless Sockets & Reliable UDP (RUDP)]]` in `Engineers-Playbook/02 Permanent/`
