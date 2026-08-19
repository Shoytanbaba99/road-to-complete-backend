# Week 3 - Day 6 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Flow Control (`rwnd`), Congestion Control (`cwnd` / AIMD), 4-Way Teardown Handshake (`FIN`/`ACK`), `TIME_WAIT` State, and TCP Keep-Alive Probing (`server.c` & `client.c`).

---

## 🌐 TCP Flow Control & Teardown State Machine

```text
[ FLOW CONTROL (Buffer Backpressure) ]
  Sender                                           Receiver (SO_RCVBUF = 2048B)
    │── Data Chunk (512B) ──────────────────────────►│ Socket Buffer fills
    │◄── ACK (rwnd = 1536B) ────────────────────────┤
    │── Data Chunk (1536B) ─────────────────────────►│ Buffer FULL (0 bytes free!)
    │◄── ACK (rwnd = 0B - Zero Window!) ────────────┤
    │ (Sender BLOCKS send() until window opens)     │

[ 4-WAY CONNECTION TEARDOWN ]
  Active Close                                     Passive Close
    │── 1. FIN (Seq = X) ───────────────────────────►│ (Enters CLOSE_WAIT)
    │◄── 2. ACK (Ack = X + 1) ──────────────────────┤
    │                                               │
    │◄── 3. FIN (Seq = Y) ──────────────────────────┤
    │── 4. ACK (Ack = Y + 1) ───────────────────────►│ (Enters LAST_ACK ➔ CLOSED)
    │
    └── Enters TIME_WAIT State (2 * MSL = 60s)
```

---

## 1. Flow Control (`rwnd` / Receive Window)
- **Purpose:** Protects the **Receiver's Socket Buffer** (`SO_RCVBUF`) from being flooded.
- **Zero Window Probe:** When `rwnd = 0`, the sender pauses transmission and periodically sends 1-byte probes until `rwnd > 0`.

---

## 2. Congestion Control (`cwnd` / AIMD)
- **Purpose:** Protects intermediate **Routers & Network Links** from link saturation.
- **Slow Start:** Exponential growth of congestion window (`cwnd`) starting from Initial Window size ($IW$).
- **Congestion Avoidance (AIMD):** Linear Additive Increase when approaching link capacity; Multiplicative Decrease (halving `cwnd`) upon packet loss.

---

## 3. Teardown & `TIME_WAIT` State
- **4-Way Teardown:** Active Close sends `FIN` $\rightarrow$ Passive ACK $\rightarrow$ Passive `FIN` $\rightarrow$ Active ACK.
- **`TIME_WAIT` State:** Active close side stays in `TIME_WAIT` for 2 Maximum Segment Lifetimes ($2 \times MSL \approx 60$s). Guarantees that late duplicate packets die in the network and the final ACK is acknowledged.
