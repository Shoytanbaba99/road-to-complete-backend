### 1. Conceptual DNS Resolution Trace

The resolution of a domain name is a strict, iterative traversal of the DNS hierarchy, initiated when a queried record is missing from local memory.

- **Stub Resolver:** The OS checks its local cache. On a miss, it dispatches a recursive query to the configured network Recursive Resolver (e.g., your ISP or 8.8.8.8).
- **Root Server (`.`):** The recursive resolver queries one of the 13 logical Root server clusters. The Root does not hold the IP; it returns a _referral_ (NS records) pointing to the Top-Level Domain (TLD) servers (e.g., the `.com` infrastructure).
- **TLD Server:** The resolver queries the TLD server. The TLD returns another referral, pointing to the specific Authoritative Nameservers registered for the target domain.
- **Authoritative Server:** The resolver queries the authoritative server. This server provides the absolute Answer (the requested Resource Record). The recursive resolver caches this record and forwards it back to the stub resolver.

### 2. Time-To-Live (TTL) & Caching Mechanics

DNS caching operates across multiple layers (browser, OS, recursive resolver) to minimize latency and reduce load on authoritative infrastructure.

- **TTL Definition:** A 32-bit unsigned integer attached to every Resource Record. It dictates the maximum time, in seconds, a record may be temporarily stored in a cache before it must be discarded.
- **Decrement Mechanism:** When a recursive resolver caches a record (e.g., starting with a TTL of 3600), it actively decrements this integer. A client querying that resolver 10 minutes later will receive the exact same record, but the attached TTL will read 3000.
- **Propagation Delay:** If an administrator updates a record on the authoritative server, any resolver that has already cached the old record will continue to serve the stale data until its internal TTL counter reaches zero.

### 3. The `dig` Utility

`dig` (Domain Information Groper) is a standard diagnostic tool that explicitly bypasses the OS's stub resolver. It manually constructs raw DNS query packets and parses the byte-level responses.

- **Packet Construction:** It natively crafts a UDP query directed to port 53.
- **Header Analysis:** The `dig` output exposes the protocol state, such as `status: NOERROR` (success) or `NXDOMAIN` (domain does not exist). It also exposes binary flags like `rd` (recursion desired) and `ra` (recursion available).
- **Iterative Tracing:** Executing `dig +trace` overrides default behavior. It forces `dig` to disable its own recursion request and manually perform the iterative Root-to-TLD-to-Authoritative traversal, printing the raw response from each tier.

?

### Deep Dive: `dig` Anatomy, TTL Mechanics, and Resolution Tracing

`dig` (Domain Information Groper) is the definitive diagnostic tool for DNS. Unlike high-level wrappers like `ping` or `host` (which rely on OS-level `getaddrinfo` abstractions and hide network-layer metadata), `dig` constructs raw DNS query packets, sends them directly to a target nameserver via UDP/TCP, and prints the raw DNS response packet parsed section-by-section.

---

### 1. Dissecting the Raw `dig` Output

When you run a standard `dig` query:

```bash
dig example.com

```

`dig` outputs the binary DNS response formatted into six distinct structural blocks:

```text
; <<>> DiG 9.18.28-0ubuntu0.24.04.1-Ubuntu <<>> example.com
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 41258
;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 1

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 1232
;; QUESTION SECTION:
;example.com.			IN	A

;; ANSWER SECTION:
example.com.		3125	IN	A	93.184.216.34

;; Query time: 24 msec
;; SERVER: 127.0.0.53#53(127.0.0.53) (UDP)
;; WHEN: Wed Aug 19 17:33:11 +06 2026
;; MSG SIZE  rcvd: 56

```

#### Header & Flags Section

- **`opcode: QUERY`:** The DNS operation type (standard query).
- **`status: NOERROR`:** The 4-bit `RCODE` returned by the server. Possible values:
- `NOERROR` (0): Query completed successfully.
- `NXDOMAIN` (3): The domain name does not exist.
- `SERVFAIL` (2): The nameserver failed to obtain a valid answer (e.g., DNSSEC validation failure, upstream timeout).
- `REFUSED` (5): The nameserver policy refused to answer (e.g., recursive query sent to an authoritative-only server with recursion disabled).

- **`id: 41258`:** The 16-bit Transaction ID matching the request to this specific response.
- **`flags` (1-bit binary flags in the DNS header):**
- `qr` (Query/Response Flag): `1` indicates this packet is a Response.
- `rd` (Recursion Desired): Set by the client to request that the server resolve iteratively on its behalf.
- `ra` (Recursion Available): Set by the server confirming it supports recursive lookups.
- `aa` (Authoritative Answer): **Missing here.** This proves the answer was returned by a recursive resolver's **cache**, not by the domain's authoritative nameserver.
- `tc` (Truncated): Set to `1` if the payload exceeded the maximum UDP buffer size, forcing a TCP retry on port 53.

#### The Section Counters

- `QUERY: 1`: Number of records in the Question section.
- `ANSWER: 1`: Number of resource records answering the question.
- `AUTHORITY: 0`: Number of authoritative nameserver referral records included.
- `ADDITIONAL: 1`: Number of metadata records (e.g., EDNS0 buffer options or glue IPs).

#### The Resource Record Line Anatomy

```text
example.com.        3125    IN    A    93.184.216.34
     ▲               ▲      ▲     ▲          ▲
   Owner            TTL   Class  Type      RDATA

```

- **Owner (FQDN):** The fully qualified domain name, terminated by the root dot (`.`).
- **TTL (Time to Live):** `3125` seconds remaining before the recursive resolver evicts this entry from its memory.
- **Class:** `IN` (Internet standard).
- **Type:** `A` (IPv4 address mapping).
- **RDATA:** The resolved payload (`93.184.216.34`).

---

### 2. How DNS Caching and TTL Decrementation Work

DNS caching operates as a distributed, multi-tiered hierarchy with independent cache layers:

```
[ Browser / App Cache ]  (Chrome: chrome://net-internals/#dns)
         │  (Cache Miss)
         ▼
[ OS-Level Stub Cache ]  (systemd-resolved / nscd / DnsCache service)
         │  (Cache Miss)
         ▼
[ Local Router / Gateway Cache ] (dnsmasq on 192.168.1.1)
         │  (Cache Miss)
         ▼
[ Recursive Resolver Cache ] (1.1.1.1 / 8.8.8.8 / ISP Resolver)
         │  (Cache Miss - Initiates Iterative Walk)
         ▼
[ Authoritative Server ] (Master Zone File - Fixed Original TTL)

```

#### The TTL Lifetime Cycle

```
Authoritative Server                     Recursive Resolver Cache                 Client Machine
(Origin Zone File)
        │                                           │                                    │
 [TTL: 3600 (Fixed)]                                │                                    │
        │── 1. Iterative Query (Cache Miss) ───────►│                                    │
        │   Returns A = 93.184.216.34, TTL = 3600   │                                    │
        │                                           │ [Stores in RAM: Expiry = T + 3600] │
        │                                           │── 2. Returns TTL = 3600 ──────────►│
        │                                           │                                    │
        │                                           │ ... 600 Seconds Pass ...           │
        │                                           │                                    │
        │                                           │◄── 3. Client B Query ──────────────┤
        │                                           │── 4. Returns TTL = 3000 ──────────►│
        │                                           │                                    │
        │                                           │ ... 3000 Seconds Pass ...          │
        │                                           │                                    │
        │                                           │ [TTL reaches 0 -> Entry Evicted]   │
        │                                           │                                    │
        │◄── 5. New Iterative Query (Cache Miss) ───│◄── 6. Client C Query ──────────────┤
        │    Returns A = 93.184.216.34, TTL = 3600  │                                    │

```

1. **At the Authoritative Nameserver:** The TTL is a static configuration parameter in the zone file (e.g., `example.com. 3600 IN A 93.184.216.34`). Every time the authoritative nameserver answers, it sends `3600`.
2. **At the Recursive Resolver:** The moment the resolver receives the record, it records an absolute timestamp:

$$\text{Eviction Time} = \text{Current Unix Epoch} + \text{Received TTL}$$

When subsequent clients query the recursive resolver, the resolver computes the remaining TTL on the fly:

$$\text{Returned TTL} = \text{Eviction Time} - \text{Current Unix Epoch}$$

The TTL strictly counts down toward zero. 3. **Cache Eviction:** Once the TTL reaches `0`, the record is purged from RAM. The next query forces a brand-new iterative walk to the authoritative server.

#### Negative Caching (RFC 2308)

When a domain does not exist (`NXDOMAIN`), the recursive resolver does not query the authoritative server repeatedly on every incoming request. It caches the _absence_ of the domain.

- The authoritative server supplies a **Start of Authority (SOA)** record in the Authority section of the `NXDOMAIN` response.
- The last field of the SOA record is the **Minimum TTL** (Negative Cache TTL).
- The resolver caches the `NXDOMAIN` response for that exact duration.

---

### 3. Tracing Resolution Conceptually with `dig +trace`

When you issue a standard query, the recursive resolver hides the entire resolution tree from you. The `+trace` argument instructs `dig` to act as an iterative resolver itself: it fetches the hardcoded Root Hints, queries a Root Server directly, follows the NS referral to the TLD Server, follows the next NS referral to the Authoritative Server, and finally retrieves the answer.

```
                          ROOT ZONE (.)
                   [a.root-servers.net ... m.root-servers.net]
                                │
                                │ Returns: NS records for .com
                                │ + Glue Records (IPs of a.gtld-servers.net)
                                ▼
                           TLD ZONE (.com)
                   [a.gtld-servers.net ... m.gtld-servers.net]
                                │
                                │ Returns: NS records for example.com
                                │ + Glue Records (if nameserver is under example.com)
                                ▼
                     AUTHORITATIVE ZONE (example.com)
                       [a.iana-servers.net]
                                │
                                │ Returns: A Record (93.184.216.34)
                                │ Flags: aa (Authoritative Answer)
                                ▼
                            FINAL IP

```

---

### Phase 3: The Empirical Proof

Run these diagnostic commands locally to observe caching, TTL countdowns, and iterative resolution traces.

---

#### 1. Live Observation of TTL Decrementation

Query a public recursive resolver twice in succession for a high-traffic domain:

```bash
dig @1.1.1.1 google.com

```

_Look at the ANSWER SECTION:_

```text
google.com.		247	IN	A	142.250.190.46

```

Wait 10 seconds, then re-run the exact same command:

```bash
dig @1.1.1.1 google.com

```

_Look at the ANSWER SECTION again:_

```text
google.com.		237	IN	A	142.250.190.46

```

**Empirical Proof:** The TTL decremented by exactly 10 seconds. The query time dropped to `~1-2 ms` because the resolver served the response directly from memory without traversing the internet.

Now, query Google's authoritative nameserver directly (bypassing all caches):

```bash
dig @ns1.google.com google.com

```

_Look at the ANSWER SECTION:_

```text
google.com.		300	IN	A	142.250.190.46
;; flags: qr aa rd; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 1

```

**Empirical Proof:**

1. The TTL reset to its static origin maximum (`300`).
2. The **`aa` (Authoritative Answer)** flag is present in the header.

---

#### 2. Dissecting an Iterative Trace (`dig +trace`)

Execute a complete trace and observe each hop:

```bash
dig +trace example.com

```

**Step 1: The Root Hop (`.`):**

```text
.			518400	IN	NS	a.root-servers.net.
...
.			518400	IN	NS	m.root-servers.net.
;; Received 1137 bytes from 127.0.0.53#53(127.0.0.53) in 0 ms

```

`dig` selects one of the 13 root server clusters and asks: _"Where is `example.com`?"_

**Step 2: The TLD Hop (`.com`):**

```text
com.			172800	IN	NS	a.gtld-servers.net.
...
com.			172800	IN	NS	m.gtld-servers.net.
;; Received 1177 bytes from 198.41.0.4#53(a.root-servers.net) in 32 ms

```

The Root server returns the 13 Generic Top-Level Domain (`gTLD`) servers responsible for all `.com` registrations.

**Step 3: The Authoritative Delegation Hop (`example.com`):**

```text
example.com.		172800	IN	NS	a.iana-servers.net.
example.com.		172800	IN	NS	b.iana-servers.net.
;; Received 734 bytes from 192.5.6.30#53(a.gtld-servers.net) in 45 ms

```

The `.com` TLD server returns the specific Authoritative Nameservers delegated to hold the `example.com` zone file.

**Step 4: The Final Authoritative Answer:**

```text
example.com.		86400	IN	A	93.184.216.34
;; Received 56 bytes from 199.43.135.53#53(a.iana-servers.net) in 18 ms

```

The authoritative server returns the final IP mapping with full authority.

---

#### 3. Inspecting DNSSEC & EDNS0 Buffer Extensions

Query with EDNS0 options enabled (modern default in `dig`):

```bash
dig +nocmd +noall +answer +additional example.com

```

- **EDNS0 (RFC 6891):** Expands the historical 512-byte UDP packet ceiling up to 1232–4096 bytes, preventing unnecessary fallbacks to TCP port 53 for large cryptographic DNSSEC keys.

---

### Phase 4: Architecture & Deliberate Breakage

To understand how caching and resolver failures manifest in production, analyze these 3 distinct DNS failure scenarios:

```
+-----------------------------------------------------------------------------------------+
| SOWING CHAOS: 3 DNS FAILURE MODES & SIGNALS                                             |
+---+-----------------------------+-------------------------------+-----------------------+
| # | Failure Mode / Sabotage     | Root Cause                    | What You Observe      |
+---+-----------------------------+-------------------------------+-----------------------+
| 1 | Lame Delegation             | Parent zone (TLD) delegates to| `status: SERVFAIL`.   |
|   | (Dead Nameserver)           | NS records that are dead,     | `dig` reports timeout |
|   |                             | unroutable, or not hosting    | or server failure;    |
|   |                             | the target zone.              | no `aa` answer.       |
+---+-----------------------------+-------------------------------+-----------------------+
| 2 | TTL Trapping                | Setting an excessively high   | Changes to A records  |
|   | (Stale Cache Propagation)   | TTL (e.g., 86400s / 24 hrs)   | take 24 hours to reach|
|   |                             | prior to an IP migration.     | clients; caches serve |
|   |                             |                               | dead IP addresses.    |
+---+-----------------------------+-------------------------------+-----------------------+
| 3 | Circular Glue Dependency    | NS is `ns1.foo.com` for domain| Recursive resolver    |
|   | Without Glue Records        | `foo.com`, but TLD has no A   | throws `SERVFAIL`;    |
|   |                             | record for `ns1.foo.com`.     | infinite lookup loop. |
+---+-----------------------------+-------------------------------+-----------------------+

```

#### Hands-On Verification of Failure Modes

1. **Simulate a Lame Query / Non-Existent Domain:**

```bash
dig this-domain-does-not-exist-at-all-12345.com

```

_Observe:_ `status: NXDOMAIN`, with the `.com` SOA record returned in the Authority section displaying the negative caching TTL. 2. **Simulate Querying a Non-Recursive Server with `+norecurse`:**

```bash
dig @198.41.0.4 example.com +norecurse

```

_Observe:_ `status: NOERROR`, but `ANSWER: 0` and `AUTHORITY: 13`. Because you asked a Root server with recursion disabled (`+norecurse`), it does not do the legwork; it returns only the referral NS records for `.com`.

---

### Phase 5: The Invariant & The Capstone

#### The Non-Negotiable Invariant

> **DNS caches are strictly autonomous and pull-based; there is no global push-invalidation mechanism.**
> Once a record is cached by an intermediate resolver with a TTL of $N$ seconds, the authoritative origin cannot invalidate, flush, or overwrite that record across global recursive caches until the timer independently expires.

---

#### Day 2 Capstone Project

Execute the following diagnostic sequence using `dig`:

1. **Step 1 (TTL Drift Calculation):**

- Run `dig @8.8.8.8 wikipedia.org` and note the exact TTL value in the Answer section.
- Run it again after exactly 15 seconds.
- Verify that the TTL decreased by 15.

2. **Step 2 (The Authoritative Delta):**

- Identify one of the authoritative nameservers for `wikipedia.org` using `dig wikipedia.org NS +short`.
- Query that nameserver directly for `wikipedia.org` with `+noall +comments +answer`.
- Confirm the presence of the `aa` flag and note the static origin TTL.

3. **Step 3 (The Raw Resolution Trace):**

- Run `dig +trace github.com`.
- Identify and write down the exact IP address of the root server that answered Hop 1, the TLD server that answered Hop 2, and the authoritative server that answered Hop 3.
