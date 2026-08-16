# Week 3 - Day 4 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of User Datagram Protocol (UDP), Datagram Mechanics, Connectionless Unreliable Transmissions, `SOCK_DGRAM`, Socket Timeouts (`SO_RCVTIMEO`), and Custom Reliable UDP (RUDP) Stop-and-Wait ARQ Protocol (`rudp_client.c` & `rudp_server.c`).

---

## 🌐 UDP Datagram Framing vs. RUDP Reliability Mechanics

```text
[ UNBUFFERED UDP DATAGRAM ]
 ├── Source Port (2 Bytes)  ├── Destination Port (2 Bytes)
 ├── Length      (2 Bytes)  └── Checksum         (2 Bytes)
 └── Datagram Payload (Atomic message boundary: No connection, No ACKs, No guarantees!)

[ RUDP (Reliable UDP over SOCK_DGRAM) ]
 Sender                                        Receiver
   │─── Datagram (Seq #0, 1024B Payload) ──────►│ Writes to disk
   │◄── ACK #0 ─────────────────────────────────│
   │
   │─── Datagram (Seq #1) ─────X (PACKET DROPPED)
   │ (Socket Timeout - 1.0s)
   │─── Retransmit Datagram (Seq #1) ──────────►│ Writes to disk
   │◄── ACK #1 ─────────────────────────────────│
```

---

## 1. UDP Protocol Mechanics (`SOCK_DGRAM`)
- **Connectionless:** No `connect()` handshake. Senders use `sendto(sock, buf, len, 0, &dest_addr, addrlen)`.
- **Atomic Message Boundaries:** Unlike stream-based TCP, each `sendto()` generates one distinct datagram packet.
- **Unreliable & Fast:** No sequence numbers, no ACKs, no retransmissions. Ideal for DNS, video streaming, and real-time multiplayer gaming.

---

## 2. Reliable UDP (RUDP) Capstone Mechanics
- **Sequence Numbers (`seq_num`):** Tracks packet order and detects duplicate packets.
- **Acknowledgements (`rudp_ack_t`):** Receiver sends ACK datagrams back to confirm receipt of specific sequence numbers.
- **Stop-and-Wait ARQ:** Sender waits for `ACK N` before transmitting `Seq N+1`.
- **Socket Timeout (`SO_RCVTIMEO`):** If no ACK arrives within 1.0 seconds, `recvfrom()` returns `EAGAIN`/`EWOULDBLOCK`, triggering an automatic packet retransmission loop.
