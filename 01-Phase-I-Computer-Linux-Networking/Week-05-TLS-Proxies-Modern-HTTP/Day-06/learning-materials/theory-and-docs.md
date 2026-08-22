Here is the strict theoretical breakdown for Week 5, Day 6 of your roadmap.

---

### 1. HTTP/2: Binary Framing & Multiplexing

HTTP/2 (RFC 9113) preserves HTTP/1.1 semantics (methods, status codes, headers) while replacing the plain-text wire protocol with a binary framing layer.

- **Binary Framing:** Communication is parsed into discrete, typed binary frames instead of plain ASCII text blocks. Every frame begins with a standardized 9-byte header containing Length (24-bit), Type (8-bit), Flags (8-bit), and a Stream Identifier (31-bit integer).

- **Streams:** A stream is a bidirectional, independent flow of frames within a single TCP connection. Streams initiated by clients use odd-numbered IDs; server-initiated streams use even-numbered IDs.

- **Multiplexing:** Multiple independent streams are interleaved simultaneously across a single TCP socket. A client can send Request A and Request B concurrently without waiting for sequential execution, eliminating HTTP/1.1 application-layer Head-of-Line (HoL) blocking.

- **Flow Control & Prioritization:** Each stream has its own flow-control window and numeric priority weight to allocate socket bandwidth among active transfers.

---

### 2. HPACK Header Compression

HTTP/1.1 repeated identical, uncompressed ASCII headers with every transaction. HTTP/2 uses HPACK (RFC 7541) to compress metadata.

- **Dual-Table Model:** HPACK references headers using two tables: a predefined **Static Table** of 61 common fields (e.g., `:method: GET`) and a session-scoped **Dynamic Table** that registers new headers encountered during the connection.
- **Index Transmission:** If a header is present in either table, the client transmits only its integer index rather than the raw ASCII string.
- **Huffman Encoding:** Any novel string literals are encoded via a static Huffman code, cutting header byte size by 30% to 80% on the wire.

---

### 3. HTTP/3 & QUIC Transport Architecture

While HTTP/2 solves application-layer blocking, it remains bound to TCP. If a single TCP packet drops on a congested link, the OS stops processing _all_ multiplexed streams until that byte is retransmitted (Transport-Level HoL blocking). HTTP/3 (RFC 9114) runs over QUIC (RFC 9000) to solve this.

- **UDP-Based Transport:** QUIC replaces TCP by implementing custom reliability, congestion control, and stream state machines entirely in user space over UDP.

- **Independent Stream Delivery:** Loss of a packet on Stream 1 delays only Stream 1; all other streams continue delivering bytes to the application without stalling.
- **Combined 0-RTT/1-RTT Handshake:** QUIC integrates TLS 1.3 directly into its transport handshake, negotiating connection state and symmetric cryptographic keys in a single round-trip (or zero round-trips on reconnection).
- **Connection Migration:** Connections are identified by a 64-bit Connection ID (CID) rather than the standard IP-Port 4-tuple, enabling uninterrupted data transfer when switching network interfaces (e.g., Wi-Fi to cellular).

---

### Phase 1: The Generation Trap

#### The Core Problem Statement

In Week 4, you mastered HTTP/1.1 and its transport characteristics over TCP streams. You learned that HTTP/1.1 introduced persistent connections via `Connection: keep-alive` to avoid tearing down and rebuilding TCP handshakes for every request.

However, as the modern web evolved from simple static text documents into complex web applications loading hundreds of resources (JavaScript bundles, CSS stylesheets, dozens of high-resolution images, API payloads, fonts), HTTP/1.1 collided directly with three structural physical constraints:

1. **Application-Layer Head-of-Line (HoL) Blocking:**
   HTTP/1.1 is strictly synchronous and sequential over a single TCP stream. A client sends a request (Request A), and the server must process, construct, and fully transmit the entire response (Response A) before it can start sending Response B over that same connection.

- If Request A is an expensive database query or video render that takes 3,000 ms to compute, and Request B is a tiny 100-byte CSS file ready in 2 ms, Request B is completely blocked behind Request A on the wire.
- Browsers historically worked around this by opening up to **6 parallel TCP connections per domain**. But opening 6 TCP connections multiplies the TLS handshakes, wastes OS socket memory buffers (`sk_sndbuf`, `sk_rcvbuf`), fights for network bandwidth, and exhausts server connection pools.

2. **ASCII Text Framing & Protocol Parsing Overhead:**
   HTTP/1.1 is an ASCII text-based protocol. Boundaries between lines are marked by `\r\n`, and boundaries between headers and body are marked by the character sequence `\r\n\r\n`.

- Parsing arbitrary-length ASCII text requires state-machine byte scanning inside the kernel and application memory buffers, which is CPU-inefficient compared to fixed-length binary structs.
- If a single byte in a header line length is miscalculated, the entire stream boundary is permanently lost.

3. **Massive Header Bloat & Redundancy:**
   HTTP/1.1 headers are uncompressed plain ASCII strings transmitted on _every single request and response_.

- In modern applications, an HTTP request carries 1 KB to 2 KB of headers (`User-Agent`, long authentication cookies, `Accept`, `Referer`, CORS policies).
- If a page requests 150 sub-resources, the client transmits 150 KB to 300 KB of **identical, repetitive header strings** back and forth across the wire, consuming bandwidth and wasting mobile radio power before any actual payload bytes move.

4. **Transport-Layer (TCP) Head-of-Line Blocking at the Network Level:**
   Even if the application layer attempts to multiplex multiple logical requests over a single TCP connection, TCP is an in-order byte stream abstraction.

- If a single IP packet drops somewhere on the internet, the receiver's OS TCP buffer stops and refuses to hand _any_ subsequent received bytes to the application until that dropped segment is retransmitted and acknowledged (`ACK`).
- A single lost packet on an image download halts the processing of all other unrelated API responses sharing that TCP stream.

---

#### The Challenge

If you were the systems engineer tasked with designing the next-generation protocol (HTTP/2 and HTTP/3) to eliminate application-layer Head-of-Line blocking, abolish ASCII parsing overhead, compress repetitive headers without security leaks, and bypass TCP-level packet stall bottlenecks:

**What architecture, data structures, wire-framing formats, and transport mechanisms would you invent from scratch—and precisely where, why, and how would each of your naive approaches break down under real-world network and security constraints?**

### Phase 2: The Isomorphic Anchor & Exhaustive Theory

#### The Isomorphic Physical Analogy: The Single-Track Railway vs. The Cargo Container Train

Imagine a busy logistics railway connecting an island factory to a distribution hub:

```
[ HTTP/1.1: The Sequential Single-Cargo Trains (Head-of-Line Blocking) ]
Train #1 [=== Heavy Steel Beams (3,000 lbs) ===] ──► Blocked! ──► Train #2 [= Tiny Letter (1 oz) =]
* Train #2 cannot pass Train #1 on the single track. The letter waits for the entire steel shipment to unload.

----------------------------------------------------------------------------------------------------

[ HTTP/2: The Interleaved Shipping Container Train (Multiplexing over Single TCP Track) ]
[ Stream 2: Letter Box ] [ Stream 1: Steel Crate 1 ] [ Stream 3: Image Box ] [ Stream 1: Steel Crate 2 ]
* All cargo is chopped into standardized, numbered containers (Binary Frames).
* Different shipments share the exact same train simultaneously. The letter arrives instantly.
* BUT: If a landslide hits the track, the ENTIRE train stops (TCP Head-of-Line Blocking).

----------------------------------------------------------------------------------------------------

[ HTTP/3 / QUIC: The Fleet of Independent Drone Couriers (UDP + Native Streams) ]
Drone #1 (Stream 1) ──► Flies independently
Drone #2 (Stream 2) ──► Flies independently (Arrives even if Drone #1 crashes in mid-air!)
* No single railway track. Packet loss on Stream 1 has ZERO effect on Stream 2.

```

1. **HTTP/1.1 (The Sequential Trains):**

- If you need to ship a 50-ton steel girder (a slow database response) and a 1-page letter (a tiny CSS file), Train #1 (the steel) occupies the entire track. Train #2 (the letter) is forced to sit in the station until Train #1 completes its entire journey and unloads.

2. **HTTP/2 (Standardized Container Multiplexing over a Single Track):**

- Instead of whole trains, we standardize on fixed-size shipping crates (**Binary Frames**).
- We slice the 50-ton steel girder into 1,000 small crates and stamp each crate with `Stream ID: 1`. We pack the letter into a single crate and stamp it with `Stream ID: 2`.
- We load them onto the train in an interleaved pattern: `[Crate 1 (Stream 1)]`, `[Crate 1 (Stream 2)]`, `[Crate 2 (Stream 1)]`.
- When the train reaches the station, the receiver immediately unloads `[Crate 1 (Stream 2)]` and hands the letter to the client in milliseconds, while the rest of the steel crates continue to stream in behind it.

3. **HTTP/3 / QUIC (The Autonomous Fleet over UDP):**

- In HTTP/2, if one container falls off the train, the railway authority (the OS TCP state machine) stops the entire train on the track until the missing crate is retrieved and re-inserted.
- HTTP/3 replaces the single railway with a fleet of independent delivery vehicles moving over an open highway (**UDP**). Each shipment stream manages its own packet tracking. If Vehicle #1 blows a tire, Vehicle #2 overtakes it and delivers its cargo without waiting.

---

### Exhaustive Technical Architecture & Wire Mechanics

---

### 1. The HTTP/2 Binary Framing Layer (RFC 7540 / RFC 9113)

HTTP/2 leaves HTTP semantics (methods `GET`/`POST`, status codes `200`/`404`, headers, URIs) unchanged, but completely replaces the transport serialization format. It introduces the **Binary Framing Layer** between the socket and the application logic.

```
                              HTTP/2 BINARY FRAME WIRE LAYOUT

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                 Length (24 bits)              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   Type (8)    |   Flags (8)   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|R|                 Stream Identifier (31 bits)                 |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                     Frame Payload (0...N)                   ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```

#### Dissecting the 9-Byte Fixed Frame Header:

- **Length (24 bits):** The length of the frame payload in bytes (default maximum is $2^{14} = 16,384$ bytes, up to $2^{24}-1 \approx 16.7$ MB if negotiated via `SETTINGS_MAX_FRAME_SIZE`).
- **Type (8 bits):** The operational purpose of the frame:
- `0x00 DATA` — Carries raw application payload bytes (the equivalent of the HTTP body).
- `0x01 HEADERS` — Carries compressed HTTP headers and initiates a new logical stream.
- `0x02 PRIORITY` — Specifies sender-advised stream priority and dependency weighting.
- `0x03 RST_STREAM` — Immediately terminates a stream (e.g., client cancels an image download without closing the TCP socket).
- `0x04 SETTINGS` — Negotiates connection-level configuration parameters (max concurrent streams, window sizes).
- `0x06 PING` — Measures round-trip time (RTT) and performs keep-alive heartbeat checks.
- `0x07 GOAWAY` — Initiates graceful connection shutdown without dropping in-flight requests.
- `0x08 WINDOW_UPDATE` — Implements stream-level and connection-level flow control.

- **Flags (8 bits):** Boolean modifiers specific to the frame type:
- `END_STREAM (0x01)`: Indicates this frame is the final chunk the sender will transmit for this stream (half-closed state).
- `END_HEADERS (0x04)`: Indicates this frame contains the complete header block without continuation.

- **Stream Identifier (31 bits):** The unique integer ID identifying which logical conversation this frame belongs to.
- `Stream ID = 0x00`: Reserved strictly for control frames that govern the entire TCP connection (e.g., `SETTINGS`, `PING`, connection-level `WINDOW_UPDATE`).
- **Client-initiated streams** use **odd numbers** (`1, 3, 5, 7...`).
- **Server-initiated streams** (Server Push) use **even numbers** (`2, 4, 6, 8...`).

---

### 2. Streams, Multiplexing, and Flow Control

```
                      HTTP/2 MULTIPLEXING OVER A SINGLE TCP STREAM

[ CLIENT APPLICATION ]                                           [ SERVER APPLICATION ]
 Stream 1: GET /app.js  ──┐                                 ┌──► Processes /app.js
 Stream 3: GET /style.css ┼─► [ HTTP/2 Framing Layer ] ────►┼──► Processes /style.css
 Stream 5: POST /data   ──┘                                 └──► Processes /data
                                       │
                                       ▼ (Single TCP Stream)
 +------------------------------------------------------------------------------------+
 | [Hdr: Stream 1] [Hdr: Stream 3] [DATA: Stream 1] [Hdr: Stream 5] [DATA: Stream 3]  |
 +------------------------------------------------------------------------------------+

```

#### Stream Lifecycle & States

Each HTTP/2 stream moves through a formal state machine:

$$\text{idle} \xrightarrow{\text{Send/Recv HEADERS}} \text{open} \xrightarrow{\text{Send END\_STREAM}} \text{half-closed} \xrightarrow{\text{Recv END\_STREAM}} \text{closed}$$

- **Independent Stream Concurrency:** The client can dispatch 100 requests simultaneously over one TCP socket. The server processes them in parallel and emits response frames in whatever order they become ready. A fast 100-byte response on Stream 3 flies past a slow 10 MB download on Stream 1.
- **Flow Control (Credit-Based):** Unlike HTTP/1.1 where backpressure relies entirely on the TCP window (`rwnd`), HTTP/2 implements **per-stream flow control** using `WINDOW_UPDATE` frames. A client can tell the server: _"Give me at most 64 KB for the background video (Stream 1), but send unlimited data for the critical stylesheet (Stream 3)."_

---

### 3. HPACK: Stateful Header Compression (RFC 7541)

In HTTP/1.1, repeating headers (`User-Agent`, cookies, `Accept`) wastes substantial bandwidth. Naive compression (like standard GZIP/Deflate) was banned due to the **CRIME vulnerability** (where an attacker observing compressed byte lengths can decrypt session cookies).

HTTP/2 uses **HPACK**, which is secure against length-oracle attacks and provides significant compression:

```
                                  HPACK ENCODING ARCHITECTURE

 [ Outbound Header Block ]
 ┌──────────────────────────────────────┐
 │ :method: GET                         │ ──► Matched in Static Table (Index 2)  ──► Emit: 0x82 (1 Byte!)
 │ :path: /index.html                   │ ──► Literal with Incremental Indexing ──► Emit: String + Add to Dynamic
 │ cookie: sid=a98f2c                   │ ──► Huffman Encoded String (RFC 7541 Table)
 └──────────────────────────────────────┘
                                           │
                                           ▼
             ┌───────────────────────────────────────────────────────────┐
             │ STATIC TABLE (Pre-defined in RFC 7541 - 61 Common Headers)│
             │ Index 2  = :method: GET                                   │
             │ Index 7  = :scheme: https                                 │
             │ Index 14 = :status: 200                                   │
             ├───────────────────────────────────────────────────────────┤
             │ DYNAMIC TABLE (In-Memory FIFO synchronized on both ends)  │
             │ Index 62 = cookie: sid=a98f2c                             │
             │ Index 63 = user-agent: Mozilla/5.0...                     │
             └───────────────────────────────────────────────────────────┘

```

1. **The Static Table:** A fixed, unchangeable table of 61 common header fields defined directly in the RFC. Transmitting `:method: GET` requires sending only the single byte index integer `0x82`.
2. **The Dynamic Table:** A FIFO buffer maintained in memory by both client and server. When an un-indexed header (like a custom cookie) is sent, the sender adds it to its local dynamic table, and the receiver adds it to its mirrored dynamic table. Subsequent requests refer to that cookie using an index number.
3. **Static Huffman Coding:** Strings that must be transmitted are compressed using a static Huffman code optimized for HTTP header character distributions.

---

### 4. HTTP/3 & The QUIC Transport Architecture (RFC 9000 / RFC 9114)

Why was HTTP/3 needed if HTTP/2 multiplexed streams?

```
+---------------------------------------------------------------------------------------------------+
| LAYER / FEATURE       | HTTP/1.1            | HTTP/2                | HTTP/3 / QUIC               |
+-----------------------+---------------------+-----------------------+-----------------------------+
| Transport Layer       | TCP (RFC 793)       | TCP (RFC 793)         | QUIC over UDP (RFC 9000)    |
+-----------------------+---------------------+-----------------------+-----------------------------+
| Encryption Layer      | Optional TLS (L6)   | Optional TLS (L6)     | Mandatory TLS 1.3 Integrated|
+-----------------------+---------------------+-----------------------+-----------------------------+
| Framing               | Plaintext ASCII     | Binary Frames         | Binary Frames (QUIC Frames) |
+-----------------------+---------------------+-----------------------+-----------------------------+
| Header Compression    | None                | HPACK (Stateful FIFO) | QPACK (Non-blocking Out-of- |
|                       |                     |                       | order compression)          |
+-----------------------+---------------------+-----------------------+-----------------------------+
| Handshake Latency     | 2-3 RTT (TCP + TLS) | 2-3 RTT (TCP + TLS)   | 0-RTT / 1-RTT (Combined)    |
+-----------------------+---------------------+-----------------------+-----------------------------+
| Head-of-Line Blocking | Application + TCP   | TCP-Level Only        | Completely Eliminated       |
+---------------------------------------------------------------------------------------------------+

```

```
               TRANSPORT STACK COMPARISON: HTTP/2 VS HTTP/3

      +---------------------+                    +---------------------+
      |   HTTP/2 (App)      |                    |   HTTP/3 (App)      |
      +---------------------+                    +---------------------+
      |   TLS 1.2 / 1.3     |                    |   QUIC (Streams,    |
      +---------------------+                    |   Encryption, Acks) |
      |   TCP (Transport)   |                    +---------------------+
      +---------------------+                    |   UDP (Datagrams)   |
      |   IP (Network)      |                    +---------------------+
      +---------------------+                    |   IP (Network)      |
                                                 +---------------------+

```

#### Core Innovations of QUIC / HTTP/3:

1. **Elimination of Transport HoL Blocking:**

- QUIC implements multiplexed streams **natively at the transport layer**.
- If a packet carrying data for Stream 1 is lost over the wireless network, the OS UDP socket continues delivering packets for Stream 3 and Stream 5 to the application immediately without waiting for Stream 1's retransmission.

2. **0-RTT / 1-RTT Unified Handshake:**

- Because QUIC integrates TLS 1.3 directly into its transport frame handshake, establishing a connection and negotiating encryption keys occurs in a single round trip (1-RTT) instead of separate TCP and TLS handshakes. For repeat connections, QUIC supports **0-RTT connection resumption**.

3. **Connection Migration (Surviving IP/Network Changes):**

- TCP connections are identified by a 4-tuple: `(Source IP, Source Port, Dest IP, Dest Port)`. If a mobile user walks out of their house and switches from Wi-Fi to 4G/5G, the phone's IP address changes. In TCP, this breaks the 4-tuple; all active sockets die, forcing full reconnects.
- QUIC identifies connections using a random **Connection ID (CID)** independent of IP or port. When your IP address changes, the client transmits its existing CID from the new IP, and the QUIC server continues streaming data seamlessly with zero interruption.

---

### How QUIC Handshakes Work: 1-RTT & 0-RTT

In standard TCP + TLS 1.3, connection setup requires separate, sequential handshakes across different layers of the operating system:

```
TCP + TLS 1.3 (2 Round Trips):
Client                                                Server
  │─── 1. TCP SYN ──────────────────────────────────────►│ ┐
  │◄── 2. TCP SYN-ACK ───────────────────────────────────┤ ┴─ [1st RTT: Layer 4 Transport Ready]
  │─── 3. TCP ACK + TLS ClientHello (Key Share) ────────►│ ┐
  │◄── 4. TLS ServerHello + Encrypted Extensions ────────┤ ┴─ [2nd RTT: Layer 6 Encryption Ready]
  │─── 5. HTTP GET (Encrypted Application Data) ────────►│ ── [Data finally moves!]

```

#### 1. QUIC 1-RTT (Initial Connection)

QUIC merges Layer 4 (Transport) and Layer 6 (Security) into a single atomic handshake over UDP:

```
QUIC Initial Handshake (1 Round Trip):
Client                                                Server
  │─── 1. UDP Datagram: [QUIC Initial + TLS 1.3 ClientHello (ECDHE Key Share)] ──►│
  │                                                                                │ [Derives Keys]
  │◄── 2. UDP Datagram: [QUIC Handshake + TLS 1.3 ServerHello (ECDHE Key Share)] ─┤
  │                                                                                │
  │   [Both sides have derived symmetric session keys after exactly 1 RTT]        │
  │                                                                                │
  │─── 3. UDP Datagram: [QUIC Short Header: Encrypted HTTP/3 GET /index] ────────►│

```

- **Why it works:** The very first UDP packet contains both the transport session identifiers (Connection ID) **and** the cryptographic Diffie-Hellman public key shares. There is no separate TCP handshake step.

#### 2. QUIC 0-RTT (Connection Resumption)

When a client connects to a server it visited recently:

```
QUIC 0-RTT Resumption (Zero Round Trips for Data):
Client                                                Server
  │─── 1. UDP Packet 1: [0-RTT Encrypted HTTP/3 GET /api/data + ClientHello] ────►│ [Processes GET immediately!]
  │◄── 2. UDP Packet 1: [Encrypted HTTP/3 200 OK Response + ServerHandshake] ─────┤

```

- **How it does this:**

1. During the _previous_ session, the server issued the client a cryptographically sealed session ticket (a **Pre-Shared Key / PSK**).
2. When reconnecting, the client derives an encryption key from that stored PSK and encrypts the HTTP request body _before even talking to the server_.
3. The client transmits the encrypted HTTP request **inside the very first outbound UDP datagram**.
4. The server decrypts and processes the request on packet arrival.

- **The Production Catch (Replay Attacks):**
  Because 0-RTT data is sent without an interactive cryptographic challenge, an eavesdropper could capture that raw UDP packet and replay it 100 times to the server.
- **The Rule:** 0-RTT is permitted **only for safe, idempotent requests (`GET`)**. RFC 9000 strictly forbids using 0-RTT for state-mutating requests (`POST`, payment processing, money transfers) unless application-level anti-replay tokens are implemented.

---

### Phase 3: The Empirical Proof

Run these diagnostic commands locally to observe ALPN negotiation, HTTP/2 binary multiplexing, and HPACK header savings directly.

---

#### 1. Inspecting ALPN (Application-Layer Protocol Negotiation) via `curl -v`

When establishing a TLS connection, the client and server negotiate which HTTP version to speak inside the TLS handshake using the **ALPN extension**:

```bash
curl -v --http2 https://www.google.com -o /dev/null

```

**Dissecting the Output:**

```text
* ALPN: offers h2, http/1.1
* TLSv1.3 (OUT), TLS handshake, Client hello (1):
* TLSv1.3 (IN), TLS handshake, Server hello (2):
...
* ALPN: server accepted h2
* Using HTTP2, server supports multiplexing
* Copying HTTP/2 data in stream 1 to buffer
* [HTTP/2] [1] OPENED stream for https://www.google.com/
* [HTTP/2] [1] [:method: GET]
* [HTTP/2] [1] [:path: /]
* [HTTP/2] [1] [:scheme: https]
* [HTTP/2] [1] [:authority: www.google.com]
* [HTTP/2] [1] [user-agent: curl/8.5.0]
* [HTTP/2] [1] [accept: */*]
< HTTP/2 200

```

- **`ALPN: offers h2, http/1.1`:** The client announces it supports both HTTP/2 (`h2`) and HTTP/1.1.
- **`ALPN: server accepted h2`:** The server selects `h2`.
- **`[HTTP/2] [1] OPENED stream`:** The client assigns **Stream ID 1** to this request.
- **Pseudo-Headers (`:method`, `:path`, `:scheme`, `:authority`):** These replace the old HTTP/1.1 ASCII Request-Line (`GET / HTTP/1.1`).

---

#### 2. Demonstrating Multiplexing vs Sequential HTTP/1.1 via `nghttp`

If you have `nghttp2-client` installed (`sudo apt install nghttp2-client` or `brew install nghttp2`), inspect real-time frame multiplexing and HPACK compression:

```bash
# Fetch a resource and display all binary frames and HPACK compression ratios
nghttp -uv https://nghttp2.org/

```

**Output Inspection:**

```text
[  0.042] send SETTINGS frame <length=18, flags=0x00, stream_id=0>
[  0.045] recv SETTINGS frame <length=36, flags=0x00, stream_id=0>
[  0.045] send HEADERS frame <length=39, flags=0x05, stream_id=1>
          ; END_STREAM | END_HEADERS
          (padlen=0)
          :method: GET
          :path: /
          :scheme: https
          :authority: nghttp2.org
[  0.078] recv HEADERS frame <length=240, flags=0x04, stream_id=1>
          ; END_HEADERS
          :status: 200
          content-type: text/html; charset=UTF-8
[  0.079] recv DATA frame <length=6520, flags=0x00, stream_id=1>
[  0.080] recv DATA frame <length=1344, flags=0x01, stream_id=1>
          ; END_STREAM

```

- Notice the exact frame sequence: `SETTINGS (Stream 0)` $\rightarrow$ `HEADERS (Stream 1)` $\rightarrow$ `DATA (Stream 1)`.
- At the bottom, `nghttp` outputs the **HPACK savings**:
  `[Space saved: 68.45% by HPACK (Header table size: 4096)]`

---

### Phase 4: Architecture & Deliberate Breakage

To understand the binary framing layer, we will construct a lightweight Python script that parses and generates raw **HTTP/2 Binary Frames** directly against a byte buffer.

#### The Binary Framing Engine (`h2_framer.py`)

```python
import struct

# Frame Types
FRAME_DATA = 0x00
FRAME_HEADERS = 0x01
FRAME_SETTINGS = 0x04
FRAME_RST_STREAM = 0x03

# Flags
FLAG_END_STREAM = 0x01
FLAG_END_HEADERS = 0x04

def pack_h2_frame(frame_type: int, flags: int, stream_id: int, payload: bytes) -> bytes:
    """
    Constructs a 9-byte HTTP/2 frame header + payload.
    Header format: Length (24-bit), Type (8-bit), Flags (8-bit), R+StreamID (32-bit).
    """
    length = len(payload)
    if length > 0xFFFFFF:
        raise ValueError("Payload exceeds 24-bit max frame size")

    # Pack: 24-bit length (as 3 bytes), 1-byte type, 1-byte flags
    length_bytes = length.to_bytes(3, byteorder='big')
    header_start = struct.pack("!BB", frame_type, flags)

    # 31-bit Stream ID (Mask out reserved top bit)
    stream_id_bytes = struct.pack("!I", stream_id & 0x7FFFFFFF)

    return length_bytes + header_start + stream_id_bytes + payload

def unpack_h2_frame(raw_bytes: bytes) -> tuple[int, int, int, bytes, bytes]:
    """
    Unpacks a 9-byte HTTP/2 frame header from a byte buffer.
    Returns: (frame_type, flags, stream_id, payload, remaining_buffer)
    """
    if len(raw_bytes) < 9:
        raise ValueError("Incomplete frame header (less than 9 bytes)")

    # Read 24-bit length
    length = int.from_bytes(raw_bytes[0:3], byteorder='big')
    frame_type, flags = struct.unpack("!BB", raw_bytes[3:5])
    stream_id = struct.unpack("!I", raw_bytes[5:9])[0] & 0x7FFFFFFF

    total_frame_len = 9 + length
    if len(raw_bytes) < total_frame_len:
        raise ValueError(f"Buffer has {len(raw_bytes)} bytes, need {total_frame_len}")

    payload = raw_bytes[9:total_frame_len]
    remaining = raw_bytes[total_frame_len:]
    return frame_type, flags, stream_id, payload, remaining

def run_simulation():
    print("=== 1. ENCODING MULTIPLEXED HTTP/2 FRAMES ===")

    # Create frames for two separate concurrent streams
    # Stream 1: GET /large-video (Chunk 1)
    frame1 = pack_h2_frame(FRAME_DATA, 0x00, 1, b"[Video Bytes Part 1]")

    # Stream 3: GET /style.css (Small file, complete with END_STREAM)
    frame2 = pack_h2_frame(FRAME_DATA, FLAG_END_STREAM, 3, b"body { background: #000; }")

    # Stream 1: GET /large-video (Chunk 2, complete with END_STREAM)
    frame3 = pack_h2_frame(FRAME_DATA, FLAG_END_STREAM, 1, b"[Video Bytes Part 2]")

    # Interleave all frames across a single continuous byte stream (Single TCP pipe)
    tcp_wire_buffer = frame1 + frame2 + frame3
    print(f"Total Multiplexed Wire Bytes: {len(tcp_wire_buffer)}")

    print("\n=== 2. RECEIVER DEMULTIPLEXING FRAMES ===")
    buffer = tcp_wire_buffer
    stream_data = {}

    while buffer:
        ftype, flags, stream_id, payload, buffer = unpack_h2_frame(buffer)
        if stream_id not in stream_data:
            stream_data[stream_id] = []
        stream_data[stream_id].append(payload)

        is_ended = bool(flags & FLAG_END_STREAM)
        print(f"Read Frame -> Type: {ftype:#04x}, Stream ID: {stream_id}, Payload: {payload.decode('latin1')}, Stream Finished: {is_ended}")

    print("\n=== 3. RECONSTRUCTED STREAMS ===")
    for sid, chunks in stream_data.items():
        full_body = b"".join(chunks)
        print(f"Stream {sid} Final Reassembled Body: {full_body.decode('latin1')}")

if __name__ == "__main__":
    run_simulation()

```

---

#### 3 Ways to Inject Failure & Observe the Breakage

```
+-----------------------------------------------------------------------------------------+
| SOWING CHAOS: 3 HTTP/2 & HTTP/3 FAILURE EXPERIMENTS                                     |
+---+-----------------------------+-------------------------------+-----------------------+
| # | Sabotage Action             | Protocol Root Cause           | What You Observe      |
+---+-----------------------------+-------------------------------+-----------------------+
| 1 | Stream ID Parity Violation  | RFC 7540 Invariant Violation  | Server sends `GOAWAY` |
|   | Client initiates request on | Clients MUST use odd stream   | frame with error code |
|   | an even stream ID (e.g., 2).| IDs; evens reserved for server| `PROTOCOL_ERROR`.     |
+---+-----------------------------+-------------------------------+-----------------------+
| 2 | Incomplete Frame Stream Cut | Binary Framing Boundary Loss  | Parser throws buffer  |
|   | Truncate buffer by 2 bytes  | Frame length field mismatch   | underrun error;       |
|   | in middle of a frame.       | cannot locate next header.    | connection hangs.     |
+---+-----------------------------+-------------------------------+-----------------------+
| 3 | 0-RTT Replay Exploitation   | Anti-Replay Defense Missing   | Attacker replays 0-RTT|
|   | Transmit non-idempotent     | Server executes non-idempotent| packet; database state|
|   | `POST /charge` over 0-RTT.  | action multiple times.        | duplicated twice.     |
+---+-----------------------------+-------------------------------+-----------------------+

```

#### Executing the Sabotage Tests Live

**Experiment 1: Stream ID Parity Rejection**
Modify `h2_framer.py` to pack a request frame with `stream_id = 2` (an even number):

```python
bad_frame = pack_h2_frame(FRAME_HEADERS, FLAG_END_HEADERS, 2, b"headers")

```

_In a real HTTP/2 server (like NGINX):_ The server immediately terminates the entire TCP connection with a `GOAWAY` frame containing `PROTOCOL_ERROR (0x01)` because even-numbered streams are strictly reserved for server-initiated push.

**Experiment 2: Frame Header Length Corruption**
Slice 4 bytes off the end of `frame1` before passing it to `unpack_h2_frame`:

```python
corrupted_buffer = frame1[:-4] + frame2
unpack_h2_frame(corrupted_buffer)

```

_Result:_ `ValueError: Buffer has 25 bytes, need 29`. Unlike HTTP/1.1 where you can scan for the next `\r\n\r\n`, in binary framing a single corrupt length byte invalidates all subsequent frames on that TCP connection.

---

### Phase 5: The Invariant & The Capstone

#### The Non-Negotiable Invariant

> **HTTP/2 multiplexing solves Application-Layer Head-of-Line blocking, but remains fundamentally bound to TCP's in-order transport delivery.**
> If an IP packet drops on an HTTP/2 TCP connection, all logical multiplexed streams stall until that single packet is retransmitted. Only HTTP/3 (QUIC over UDP) achieves true independent stream transport.

---

#### Day 6 Capstone Challenge

1. **Step 1:** Run `curl -I --http2 [https://httpbin.org/get](https://httpbin.org/get)` and check the status line. Notice the presence of `:status: 200` instead of `HTTP/1.1 200 OK`.
2. **Step 2:** Answer in one concise sentence: Why does an HTTP/2 connection require only **one** TCP 3-way handshake to fetch 100 images concurrently, whereas HTTP/1.1 historically required opening **6** separate TCP connections?
3. **Step 3:** Explain in your own words: Why is 0-RTT connection resumption in QUIC dangerous for a `POST /checkout` transaction, but completely safe for a `GET /index.html` request?
