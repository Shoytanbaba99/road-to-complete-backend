# Week 5 - Day 6 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of HTTP/2 (Binary Framing, Stream Multiplexing, Odd/Even Stream IDs, HPACK Header Compression, Server Push) and HTTP/3 / QUIC (UDP Transport, Head-of-Line Blocking Elimination, 0-RTT/1-RTT TLS 1.3 Handshake, Connection Migration).

---

## 🌐 HTTP/1.1 vs. HTTP/2 vs. HTTP/3 Comparison Matrix

| Architectural Feature | HTTP/1.1 | HTTP/2 | HTTP/3 |
|---|---|---|---|
| **Transport Protocol** | TCP | TCP | **QUIC (over UDP)** |
| **Wire Format** | ASCII Text (`\r\n\r\n`) | Binary Framing | Binary Framing |
| **Multiplexing** | ❌ Head-of-Line Blocking | ✅ Stream Multiplexing | ✅ Per-Stream Loss Isolation |
| **Header Compression** | ❌ None | ✅ HPACK (Static/Dynamic) | ✅ QPACK |
| **Handshake Latency** | 2–3 RTT (TCP + TLS) | 2–3 RTT (TCP + TLS) | **1 RTT / 0 RTT (Combined QUIC+TLS 1.3)** |
| **Network Mobility** | ❌ Broken on IP Change | ❌ Broken on IP Change | **✅ Connection ID Migration** |

---

## 1. HTTP/2 Binary Framing & HPACK
- **Stream Identifiers:** Client-initiated streams use **Odd IDs** (`1, 3, 5`), Server-initiated streams use **Even IDs** (`2, 4, 6`).
- **HPACK Compression:** Eliminates duplicate header transmission using 61 static table entries (common headers like `:method GET`, `:status 200`) and connection-scoped dynamic tables.

---

## 2. HTTP/3 QUIC Innovation
- **No TCP Head-of-Line Blocking:** If a UDP packet carrying Stream 3 drops, Stream 5 and Stream 7 continue processing immediately without waiting for retransmission.
- **Connection Migration:** Connections are bound to a 64-bit Connection ID instead of the IP:Port 4-tuple, keeping socket streams alive when switching networks (Wi-Fi ➔ 5G).
