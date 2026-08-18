# Week 3 - Day 5 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Transmission Control Protocol (TCP), Three-Way Handshake (`SYN`, `SYN-ACK`, `ACK`), Sequence & Acknowledgement Numbers, Dynamic RTT / RTO Retransmission, Flow Control, and Raw TCP SYN Scanner (`syn_scan.c`).

---

## 🌐 TCP Three-Way Handshake & Sequence Number Mechanics

```text
[ CLIENT ]                                                [ SERVER ]
    │                                                         │
    ├─── 1. SYN (Seq = X) ───────────────────────────────────►│ (Server allocates TCB)
    │                                                         │
    │◄── 2. SYN-ACK (Seq = Y, Ack = X + 1) ───────────────────┤
    │                                                         │
    ├─── 3. ACK (Seq = X + 1, Ack = Y + 1) ──────────────────►│ (Connection ESTABLISHED)
    │                                                         │
    ├────────── [ Data Transmission Phase ] ──────────────────┤
    ├─── Data Segment (Seq = X + 1, Length = 100) ───────────►│
    │◄── ACK (Ack = X + 101) ─────────────────────────────────┤ (In-order byte acknowledgment)
```

---

## 1. TCP Three-Way Handshake
- **SYN (Synchronize):** Client initiates connection request with an Initial Sequence Number ($ISN_C$).
- **SYN-ACK:** Server acknowledges client ($ACK = ISN_C + 1$) and sends its own Sequence Number ($ISN_S$).
- **ACK:** Client acknowledges server ($ACK = ISN_S + 1$). Connection transitions to `ESTABLISHED`.

---

## 2. Reliability & Retransmission
- **Byte-Oriented Sequence Numbers:** Sequence numbers track exact byte offsets rather than packet count.
- **Dynamic RTT / RTO Calculations:** Measures Round-Trip Time (RTT) per segment to dynamically compute Retransmission Timeout (RTO).
- **Flow & Congestion Control:** Uses TCP Window Size (advertised buffer capacity) and Congestion Windows (`cwnd`) to avoid saturating network links or receiver buffers.

---

## 3. Raw TCP Stealth SYN Scanner ([`syn_scan.c`](learning-materials/syn_scan.c))
- Uses `SOCK_RAW` with `IP_HDRINCL` to hand-craft IP headers (`struct iphdr`) and TCP headers (`struct tcphdr`).
- Computes TCP checksum using pseudo-header (`pseudo_header` + `tcphdr`).
- Sends raw `SYN` frame without completing the 3-way handshake:
  - `SYN-ACK` response $\rightarrow$ Target port is **OPEN**!
  - `RST` (Reset) response $\rightarrow$ Target port is **CLOSED**!
