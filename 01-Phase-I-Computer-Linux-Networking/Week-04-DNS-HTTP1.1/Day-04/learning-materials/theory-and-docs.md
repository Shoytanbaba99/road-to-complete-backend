### 1. HTTP Method Semantics

HTTP methods define the exact operational semantics of a request. RFC 9110 standardizes these behaviors at the protocol layer.

| Method  | Formal Mechanism                                                                       |
| ------- | -------------------------------------------------------------------------------------- |
| **GET** | Retrieves a representation of a resource. The request must not contain a payload body. |

|
| **POST** | Appends or processes data. It allows the server to determine the specific processing rules, often resulting in new resource creation or state changes.

|
| **PUT** | Replaces the entire target resource with the exact payload provided in the request body.

|
| **PATCH** | Applies a partial set of explicit modifications to a target resource.

|
| **DELETE** | Instructs the server to remove the specified resource.

|
| **HEAD** | Identical to a GET request, but the server is strictly forbidden from returning a message body. It is used to fetch metadata.

|
| **OPTIONS** | Queries the server for the permitted communication methods available for a specific URI, crucial for CORS preflight checks.

|

### 2. Safe Methods

A method is designated as "safe" if its execution is strictly read-only and does not alter the server's state.

- **Applicability:** `GET`, `HEAD`, and `OPTIONS` are safe methods.

- **Architecture Implication:** Caching proxies, CDNs, and web crawlers depend on this safety guarantee to pre-fetch resources automatically without accidentally triggering destructive database transactions.

### 3. Idempotency Constraints

Idempotency is a mathematical property applied to distributed network systems. A method is idempotent if executing the identical request multiple times yields the exact same server state as executing it just once.

- **Idempotent Methods:** `PUT`, `DELETE`, `GET`, `HEAD`, and `OPTIONS`. For example, deleting a resource ten times leaves it deleted; replacing a resource via PUT ten times leaves the exact same data.

- **Non-Idempotent Methods:** `POST` and `PATCH`.

- **Failure Mode Recovery:** This property dictates network retry logic. If a connection drops during a `PUT`, a client can automatically retransmit the payload safely. If a connection drops during a `POST` (such as processing a payment), blind automatic retries are dangerous and risk duplicate transactions unless application-layer safeguards are implemented.

### 4. Content Negotiation

Content negotiation is the protocol mechanism allowing a single URI to serve multiple distinct representations of a resource.

- **Client Mechanism:** The client transmits preference constraints via specific headers like `Accept` (MIME types) or `Accept-Encoding` (compression algorithms).

- **Weighting:** Clients can assign relative quality values (e.g., `q=0.9`) to dictate preference hierarchies.
- **Server Resolution:** The server parses these constraints against its available representations and returns the optimal payload, explicitly declaring its chosen format in the `Content-Type` response header.

### Phase 1: The Generation Trap

#### The Core Problem Statement

In Day 3, we established the wire format of HTTP: the Start-Line, Headers, CRLF delimiter, and Message Body. However, having a mechanical framing format alone does not tell distributed systems _how to behave_ when interacting with state across an unreliable network.

Consider a distributed system where thousands of independent clients interact with stateful resources on a server across an unreliable network with arbitrary packet loss, connection drops, automated retries, intermediate caching proxies, and heterogeneous client devices:

1. **State Mutation & Network Failure Ambiguity:** When a client sends a command to create an order, charge a credit card, or update a user profile, the network might drop the request _before_ reaching the server, or drop the response _after_ the server has already executed the change. The client receives a network timeout. Should the client's automated networking layer or proxy automatically retry the exact same request? If the request was creating an order, a blind retry might double-bill the customer. If the request was simply reading data, retrying is completely harmless. How do we formally classify and guarantee which operations are safe to blindly replay across unreliable networks and which are not?
2. **Granular Resource State Semantics:** Different interactions require fundamentally different intentions:

- Fetching a resource without altering server state.
- Replacing an entire entity document wholesale.
- Modifying only a single field inside an existing entity without overwriting the rest.
- Removing an entity.
- Checking whether a massive resource has changed without wasting bandwidth downloading its body.
- Querying what capabilities, methods, or security requirements a server endpoint supports before executing an action.

If every interaction were collapsed into a single generic command (e.g., executing everything via a blind `SEND` or remote procedure call), intermediate infrastructure (caches, CDNs, API gateways, load balancers, browser history engines, crawlers) would have zero visibility into whether an operation is cacheable, retryable, destructive, partial, or read-only. 3. **Format & Representation Heterogeneity (Content Negotiation):** A single logical entity on a server (e.g., "User Profile #42") can have multiple physical representations: JSON for a Single Page App, HTML for a desktop browser, XML for a legacy enterprise service, compressed with Brotli or Gzip, written in English, French, or Bengali. The server cannot guess what the client's runtime environment can decode, and the client cannot predict all variations the server has available.

---

#### The Challenge

If you were the engineer tasked with solving these problems from scratch:

**What naive approach would you take to define operation types, handle failed/retried network requests safely, and negotiate data formats between clients and servers—and precisely where and why do you think your naive approach would break under real-world conditions?**

### Evaluation of Your Intuition

Your intuition captures the core architecture of HTTP method semantics and content handling:

1. **Safety vs. Idempotency:** You correctly recognized that `GET`, `HEAD`, and `OPTIONS` are **safe**, and that `PUT` and `DELETE` are **idempotent**.
2. **State Mutation:** You correctly identified `POST` and `PATCH` as operations that modify state without safety guarantees, meaning repeating them may trigger multiple distinct mutations or appends.
3. **Content Representation:** You noted that heterogeneity is managed via encoding and compression negotiation. Beyond compression, this mechanism—**Content Negotiation**—also handles data formats (JSON vs. XML vs. HTML), character sets, and natural human languages through prioritized preference weightings ($q$-factors).

Let us formalize these concepts into an exhaustive architectural model.

---

### Phase 2: The Isomorphic Anchor & Exhaustive Theory

#### The Isomorphic Physical Analogy: The Municipal Land & Document Archive

Imagine a centralized, physical Municipal Records Bureau where citizens interact with master ledgers stored in numbered filing cabinets:

```
[ Citizen / Contractor (Client) ]
       │
       ▼ (Submits formal transaction slip across counter)
 [ Records Bureau Counter (HTTP Server Interface) ]
       │
       ├─ [READ-ONLY / SAFE INSPECTIONS]
       │    ├─ View Deed inside Folder #42 through glass window ──────────► (GET)
       │    ├─ Check the physical thickness & date stamp on Folder #42 ────► (HEAD)
       │    └─ Ask the clerk: "What filing operations are allowed here?" ──► (OPTIONS)
       │
       ├─ [DETERMINISTIC / IDEMPOTENT MUTATIONS]
       │    ├─ Replace entire contents of Folder #42 with new Deed ───────► (PUT)
       │    └─ Stamp Folder #42 "DESTROYED" and incinerate contents ───────► (DELETE)
       │
       └─ [RELATIVE / NON-IDEMPOTENT MUTATIONS]
            ├─ Drop an unindexed invoice into the general intake hopper ──► (POST)
            └─ Instruct clerk: "Append \$50 late fee to line 12 of Deed" ──► (PATCH)

```

1. **Safe Actions (Read-Only / Non-Destructive):**

- **`GET` (Inspecting the Deed):** You ask the clerk to bring Folder #42 so you can read the text. Reading the paper changes nothing inside the folder. If 1,000 people read the folder simultaneously, the text remains identical.
- **`HEAD` (Inspecting the Binder Label):** You do not want the heavy 500-page document; you only want to see the date stamp and page count written on the exterior spine.
- **`OPTIONS` (Consulting the Rulebook):** You ask: "Can I replace files here? Can I delete files here?" The clerk hands you an authorized list of permitted actions without touching the archive.

2. **Idempotent Actions (Deterministic State Alignment):**

- **`PUT` (Wholesale Replacement):** You bring a complete, brand-new deed labeled "Owner: Alice, Value: $500k" and tell the clerk: "Wipe out whatever is currently inside Folder #42 and put this exact document in its place."
- _The Idempotency Guarantee:_ If the pneumatic delivery tube stutters and accidentally sends this exact command 10 times, Folder #42 ends up containing "Owner: Alice, Value: $500k." The end state after 1 submission is identical to the end state after 10 submissions:

$$f(f(x)) = f(x)$$

- **`DELETE` (Purging):** You tell the clerk: "Destroy Folder #42."
- _The Idempotency Guarantee:_ The first time the clerk executes the order, Folder #42 is destroyed. If the command is replayed 5 more times, the folder is _still gone_. The subsequent commands might return "Folder already missing," but the state of the archive remains unchanged.

3. **Non-Idempotent Actions (Appends & Delta Mutations):**

- **`POST` (Subordinate Ingestion):** You drop an invoice into the intake chute without a folder number. The clerk assigns it the next available ID (#101), files it, and gives you a receipt.
- _Why it breaks on replay:_ If a network glitch causes your courier to drop the exact same invoice into the chute 3 times, the clerk creates three distinct records (#101, #102, #103), tripling the billing.

- **`PATCH` (Delta / Partial Modification):** You tell the clerk: "Add $50 to the outstanding balance line in Folder #42."
- _Why it breaks on replay:_ If replayed 3 times, the clerk adds $50 three separate times, corrupting the balance.

4. **Content Negotiation (The Multi-Lingual Manifest):**

- When requesting a blueprint, you attach a specification tag:
- _"I need this in English (Preference: 100%), but French is acceptable if English is unavailable (Preference: 80%). Deliver it formatted as a technical vector diagram (JSON) rather than a hand-drawn sketch (HTML), compressed inside a sealed envelope (gzip)."_

- The clerk matches your preference list against the archived copies, selects the best match, stamps the delivery slip with the chosen format, and hands it over.

---

### Exhaustive Technical Architecture & Wire Semantics

---

### 1. The Core HTTP Methods in Detail

```
+---------------------------------------------------------------------------------------------------+
| METHOD  | RFC SPEC        | SAFE? | IDEMPOTENT? | REQUEST BODY? | RESPONSE BODY? | CACHEABLE?     |
+---------+-----------------+-------+-------------+---------------+----------------+----------------+
| GET     | RFC 9110 §9.3.1 | YES   | YES         | NO (Ignored)  | YES            | YES (Default)  |
+---------+-----------------+-------+-------------+---------------+----------------+----------------+
| HEAD    | RFC 9110 §9.3.2 | YES   | YES         | NO (Ignored)  | NO (MUST NOT)  | YES            |
+---------+-----------------+-------+-------------+---------------+----------------+----------------+
| POST    | RFC 9110 §9.3.3 | NO    | NO          | YES           | YES            | Conditional    |
+---------+-----------------+-------+-------------+---------------+----------------+----------------+
| PUT     | RFC 9110 §9.3.4 | NO    | YES         | YES           | Optional       | NO             |
+---------+-----------------+-------+-------------+---------------+----------------+----------------+
| PATCH   | RFC 5789 §2     | NO    | NO          | YES           | YES            | Conditional    |
+---------+-----------------+-------+-------------+---------------+----------------+----------------+
| DELETE  | RFC 9110 §9.3.5 | NO    | YES         | Optional      | Optional       | NO             |
+---------+-----------------+-------+-------------+---------------+----------------+----------------+
| OPTIONS | RFC 9110 §9.3.7 | YES   | YES         | Optional      | YES            | NO             |
+---------+-----------------+-------+-------------+---------------+----------------+----------------+

```

#### A. `GET`

- **Semantic Purpose:** Requests transfer of a current representation of the target resource identified by the Request-URI.
- **Payload Constraints:** An HTTP `GET` request should not contain a request body. While some server implementations tolerate a body, RFC 9110 defines no semantic meaning for it, and intermediary caching proxies may drop or reject it.
- **Caching:** Responses to `GET` are implicitly cacheable by browsers, CDNs, and reverse proxies unless explicit `Cache-Control` directives forbid it.

#### B. `HEAD`

- **Semantic Purpose:** Identical to `GET`, except the server **must not** return a message body in the response.
- **Wire Invariant:** The server returns the exact headers that a `GET` request to the same URI would produce (e.g., `Content-Length`, `Content-Type`, `ETag`, `Last-Modified`).
- **Primary Use Cases:**

1. Verifying whether a resource exists without downloading payload bytes.
2. Inspecting the `Content-Length` header to allocate disk or memory buffers prior to downloading massive files.
3. Cache validation: Checking the `ETag` or `Last-Modified` headers to determine if a locally cached copy has expired.

#### C. `POST`

- **Semantic Purpose:** Requests that the target resource process the representation enclosed in the payload according to the resource's own specific semantics.
- **Standard Operational Scenarios:**

1. Creating a subordinate resource whose URI is assigned by the server (e.g., submitting a comment to `/articles/123/comments` creates `/articles/123/comments/456`).
2. Executing an operational command or pipeline (e.g., `/rpc/calculate-taxes`).
3. Submitting web forms (`application/x-www-form-urlencoded` or `multipart/form-data`).

- **State Behavior:** Neither safe nor idempotent. Submitting the exact same `POST` request $N$ times causes $N$ discrete server-side actions unless application-level idempotency keys are enforced.

#### D. `PUT`

- **Semantic Purpose:** Requests that the state of the target resource be **created or completely replaced** with the state defined by the representation enclosed in the request message body.
- **URI vs. Payload Authority:** The URI in a `PUT` request identifies the exact resource entity itself (e.g., `PUT /users/42`). If `/users/42` exists, its entire data model is overwritten by the payload. If it does not exist, the server creates it at that exact URI (if authorized).
- **The "Full Replacement" Invariant:** If a user entity has fields `{ "name": "Alice", "age": 30, "role": "admin" }` and a client sends:

```http
PUT /users/42 HTTP/1.1
Content-Type: application/json

{ "name": "Alice" }

```

The resulting server state **must** overwrite the whole record, removing or nullifying `"age"` and `"role"`. `PUT` is not a partial update.

#### E. `PATCH` (RFC 5789)

- **Semantic Purpose:** Requests that a set of changes described in the request entity be applied to the target resource (Partial Modification).
- **Wire Format Standards:** The payload of a `PATCH` request is not a raw object; it is a specialized **diff document** defining instructions for transformation:

1. **JSON Patch (`application/json-patch+json` / RFC 6902):**

```json
[
  { "op": "replace", "path": "/email", "value": "alice@example.com" },
  { "op": "remove", "path": "/temporary_token" }
]
```

2. **JSON Merge Patch (`application/merge-patch+json` / RFC 7396):**
   Sends only the fields to be updated or overridden; keys set to `null` are deleted:

```json
{ "email": "alice@example.com", "temporary_token": null }
```

- **Idempotency Rule:** `PATCH` is **non-idempotent by default**. While a simple field assignment can happen to be idempotent, patch documents containing relative mutations (such as `{ "op": "increment", "path": "/login_count", "value": 1 }` or JSON arrays using an `add` operation at the end of a list) alter state differently on every successive replay.

#### F. `DELETE`

- **Semantic Purpose:** Requests that the origin server remove the association between the target resource and its Request-URI.
- **Response Semantics:**
- `200 OK` (if returning a status payload or deleted entity representation).
- `202 Accepted` (if the deletion is queued asynchronously).
- `204 No Content` (if the action has been enacted and no body is returned).

- **Idempotency Proof:** The first execution deletes the resource. Subsequent executions find nothing to delete (returning `404 Not Found`), but the resultant state of the server remains unchanged: the resource remains deleted.

#### G. `OPTIONS`

- **Semantic Purpose:** Requests information about the communication options available for the target resource, or for the server in general (using `OPTIONS * HTTP/1.1`).
- **The `Allow` Response Header:** The server responds with a comma-separated list of permitted HTTP verbs:

```http
HTTP/1.1 200 OK
Allow: GET, HEAD, POST, OPTIONS
Content-Length: 0

```

- **CORS Preflight Mechanism:** Modern web browsers automatically inject an `OPTIONS` request before initiating cross-origin mutations (`POST`, `PUT`, `DELETE` with custom headers) to verify server authorization before sending actual application payloads.

---

### 2. Safety vs. Idempotency Formalization

```
                          DISTRIBUTED OPERATIONS TAXONOMY

                   ┌─────────────────────────────────────────┐
                   │               ALL METHODS               │
                   │  (GET, HEAD, OPTIONS, PUT, DELETE,      │
                   │   POST, PATCH)                          │
                   └────────────────────┬────────────────────┘
                                        │
                                        ▼
                   ┌─────────────────────────────────────────┐
                   │           IDEMPOTENT METHODS            │
                   │  f(f(x)) = f(x)                         │
                   │  (GET, HEAD, OPTIONS, PUT, DELETE)      │
                   └────────────────────┬────────────────────┘
                                        │
                                        ▼
                   ┌─────────────────────────────────────────┐
                   │              SAFE METHODS               │
                   │  Read-Only / No Side Effects            │
                   │  (GET, HEAD, OPTIONS, TRACE)            │
                   └─────────────────────────────────────────┘

```

#### The Safety Contract (RFC 9110 §9.2.1)

An HTTP method is **Safe** if its semantics are essentially read-only: the client does not request, and does not expect, any state change on the origin server.

- **Why this matters to distributed architecture:**
- **Web Crawlers (Search Engines):** Googlebot or Bingbot can recursively traverse every link (`<a>` tag) across the global internet under the assumption that `GET` is safe. If a developer maps a state-changing action to `GET` (e.g., `<a href="/users/42/delete">`), a search engine crawler will purge the entire database.
- **Aggressive Caching & Pre-fetching:** Browsers can pre-fetch links in the background before a user clicks them, knowing no server state is modified.

#### The Idempotency Contract (RFC 9110 §9.2.2)

An HTTP method is **Idempotent** if the intended effect on the server of multiple identical requests with that method is the same as the effect of a single such request.

$$\text{Effect}(R_1) \equiv \text{Effect}(R_1 + R_2 + \dots + R_n)$$

- **Why this matters for network recovery:**
- If a client sends a `PUT /orders/987` or `DELETE /orders/987` and the TCP connection suddenly terminates due to a dropped packet _before the response is received_, the client cannot know whether the server executed the operation.
- Because `PUT` and `DELETE` are strictly idempotent, the client's automated networking layer can **blindly replay the exact same request** without human intervention or risk of state corruption.
- Conversely, an automated client **must never blindly retry a `POST` request** upon receiving a network timeout, because doing so risks executing duplicate operations (e.g., charging a card twice).

### [Continuation — Part 2]

---

### 3. Content Negotiation Deep Dive

Content Negotiation (RFC 9110 §12) allows a client and server to dynamically agree on the optimal representation of a resource without altering the resource's identity (its URI).

```
                               CONTENT NEGOTIATION WIRE DIALOGUE

Client (Browser / API Client)                                              Origin Server
      │                                                                           │
      │── 1. GET /reports/2026-financials ───────────────────────────────────────►│
      │   Accept: application/json;q=1.0, text/html;q=0.8, */*;q=0.1              │ [Inspects available formats]
      │   Accept-Encoding: gzip, br;q=0.9                                         │ [Matches JSON + gzip]
      │   Accept-Language: fr-FR, fr;q=0.9, en-US;q=0.8, en;q=0.7                 │ [Matches en-US fallback]
      │                                                                           │
      │◄── 2. HTTP/1.1 200 OK ────────────────────────────────────────────────────┤
      │   Content-Type: application/json; charset=utf-8                           │
      │   Content-Encoding: gzip                                                  │
      │   Content-Language: en-US                                                 │
      │   Vary: Accept, Accept-Encoding, Accept-Language                          │
      │                                                                           │
      │   [Compressed Binary Octets of JSON payload...]                           │

```

#### A. The Negotiation Dimensions & Request Headers

```
+---------------------------------------------------------------------------------------------------+
| DIMENSION           | CLIENT REQUEST HEADER | SERVER RESPONSE HEADER | EXAMPLE CLIENT VALUE       |
+---------------------+-----------------------+------------------------+----------------------------+
| Media Type (Format) | Accept                | Content-Type           | application/json, text/csv |
+---------------------+-----------------------+------------------------+----------------------------+
| Compression / Coding| Accept-Encoding       | Content-Encoding       | gzip, br, zstd, deflate    |
+---------------------+-----------------------+------------------------+----------------------------+
| Natural Language    | Accept-Language       | Content-Language       | en-US, es-ES;q=0.8         |
+---------------------+-----------------------+------------------------+----------------------------+
| Character Encoding  | Accept-Charset        | Content-Type (charset) | utf-8, iso-8859-1;q=0.5    |
+---------------------+-----------------------+------------------------+----------------------------+

```

#### B. Quality Values ($q$-factor) and Weighting Arithmetic

When a client can accept multiple formats, it declares relative preference weights using quality values ($q$).

- $q \in [0.0, 1.0]$, where $1.0$ is highest priority and default if omitted.
- Format: `Type/Subtype;q=value`

Consider:

```http
Accept: text/html, application/xhtml+xml, application/xml;q=0.9, image/webp, */*;q=0.8

```

- **Tier 1 ($q=1.0$ default):** `text/html`, `application/xhtml+xml`, `image/webp`
- **Tier 2 ($q=0.9$):** `application/xml`
- **Tier 3 ($q=0.8$):** Any other format (`*/*`)

The server executes a matching algorithm:

1. Parse client preferences into a sorted list ordered by descending $q$-score.
2. Evaluate its own available representations.
3. Select the intersection with the highest matching $q$-score.
4. If no intersection exists:

- Return `406 Not Acceptable` (Client requested format server cannot produce), OR
- Serve a default fallback representation.

5. If the client sends an unsupported body in `POST`/`PUT` (e.g., sending XML to an endpoint expecting JSON), the server returns `415 Unsupported Media Type`.

#### C. The `Vary` Header Invariant (Protecting Caches)

When a server dynamically selects a representation based on request headers, intermediate caches (CDNs, forward proxies, browser disk caches) face a data poisoning risk:

- If User 1 requests `/data` with `Accept-Language: fr`, the CDN caches the French HTML.
- If User 2 requests `/data` with `Accept-Language: en`, a naive CDN would return the cached French HTML to User 2.

**The Solution:** The server **must** return the `Vary` header:

```http
Vary: Accept, Accept-Encoding, Accept-Language

```

This instructs all intermediate caching nodes: _"Do not key this cache entry by URI alone. Key it by `(URI + Accept + Accept-Encoding + Accept-Language)`."_

---

### Phase 3: The Empirical Proof

Run these diagnostic commands locally to inspect method semantics, idempotency differences, and content negotiation algorithms on live endpoints.

---

#### 1. Verifying `HEAD` vs `GET` Invariants

Use `curl -I` (which executes a raw `HEAD` request) and compare it against `curl -i` (`GET`):

```bash
# Execute HEAD request
curl -I https://httpbin.org/get

```

**Output:**

```http
HTTP/2 200
date: Wed, 19 Aug 2026 14:48:10 GMT
content-type: application/json
content-length: 305
access-control-allow-origin: *
access-control-allow-credentials: true

```

_Notice:_ The connection immediately terminates after the header block. Zero payload bytes are downloaded, yet `content-length: 305` is accurately declared.

Now verify with a full `GET` request:

```bash
curl -i https://httpbin.org/get

```

_Notice:_ The headers match the `HEAD` output, but the socket continues reading until all 305 bytes of the JSON body are received.

---

#### 2. Querying Endpoint Capabilities via `OPTIONS`

Inspect which methods are authorized on an origin:

```bash
curl -i -X OPTIONS https://httpbin.org/anything

```

**Output:**

```http
HTTP/2 200
date: Wed, 19 Aug 2026 14:50:02 GMT
content-type: text/html; charset=utf-8
allow: GET, POST, HEAD, OPTIONS, PUT, DELETE, TRACE, PATCH
content-length: 0

```

_Notice:_ The `Allow` header explicitly advertises the valid operational verbs supported by this route.

---

#### 3. Testing Content Negotiation & $q$-Factor Sorting

Send specific preference weighting vectors to inspect how the server switches formats:

**Test A: Explicitly Request JSON**

```bash
curl -i -H "Accept: application/json" https://httpbin.org/xml

```

_Notice:_ `httpbin.org/xml` only generates XML, so it delivers XML regardless or returns standard payload with `content-type: application/xml`.

**Test B: Request Compressed Brotli / Gzip Payload**

```bash
curl -i -H "Accept-Encoding: gzip, deflate, br" https://httpbin.org/gzip

```

**Output:**

```http
HTTP/2 200
date: Wed, 19 Aug 2026 14:52:19 GMT
content-type: application/json
content-encoding: gzip
...
[Binary unprintable compressed data streamed to terminal]

```

_Notice:_ The server matches `Accept-Encoding: gzip`, applies gzip compression to the internal buffer, sets `Content-Encoding: gzip`, and ships raw compressed binary.

---

### Phase 4: Architecture & Deliberate Breakage

To ground the mental model of method safety, idempotency enforcement, and content negotiation, here is a complete Python HTTP server that implements a stateful in-memory database with strict RFC-compliant method routing and quality-factor content negotiation.

#### The Method-Strict REST Server (`rest_server.py`)

```python
import socket
import json

# In-memory database
DATABASE = {
    "1": {"id": "1", "name": "System Architecture Manual", "price": 45.00}
}

def parse_q_values(accept_header):
    """
    Parses 'Accept' header into a prioritized list sorted by q-factor.
    Example: 'application/json;q=0.9, text/html;q=1.0' -> [('text/html', 1.0), ('application/json', 0.9)]
    """
    if not accept_header:
        return [("*/*", 1.0)]

    preferences = []
    for item in accept_header.split(","):
        parts = item.strip().split(";")
        media_type = parts[0].strip()
        q = 1.0
        for param in parts[1:]:
            param = param.strip()
            if param.startswith("q="):
                try:
                    q = float(param[2:])
                except ValueError:
                    q = 1.0
        preferences.append((media_type, q))

    # Sort by q descending
    preferences.sort(key=lambda x: x[1], reverse=True)
    return preferences

def render_representation(data, accepted_types):
    """
    Content Negotiation Engine: Matches client types against available formats.
    """
    available = ["application/json", "text/html", "text/plain"]

    for media_type, q in accepted_types:
        if q == 0.0:
            continue
        if media_type == "application/json" or media_type == "*/*":
            return json.dumps(data).encode('utf-8'), "application/json"
        elif media_type == "text/html":
            html = f"<html><body><h1>Product {data.get('id')}</h1><p>Name: {data.get('name')}</p><p>Price: ${data.get('price')}</p></body></html>"
            return html.encode('utf-8'), "text/html; charset=utf-8"
        elif media_type == "text/plain":
            txt = f"ID: {data.get('id')}\nName: {data.get('name')}\nPrice: {data.get('price')}\n"
            return txt.encode('utf-8'), "text/plain; charset=utf-8"

    return None, None # 406 Not Acceptable

def start_server():
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', 8081))
    srv.listen(5)
    print("[*] REST & ConNeg Server running on http://127.0.0.1:8081")

    while True:
        conn, addr = srv.accept()
        raw = b""
        while b"\r\n\r\n" not in raw:
            chunk = conn.recv(1024)
            if not chunk:
                break
            raw += chunk

        if not raw:
            conn.close()
            continue

        header_part, _, body_part = raw.partition(b"\r\n\r\n")
        lines = header_part.decode('iso-8859-1').splitlines()
        if not lines:
            conn.close()
            continue

        method, path, version = lines[0].split(" ")
        headers = {}
        for line in lines[1:]:
            if ":" in line:
                k, v = line.split(":", 1)
                headers[k.strip().lower()] = v.strip()

        # Read remaining body if Content-Length specified
        content_len = int(headers.get("content-length", 0))
        body = body_part
        while len(body) < content_len:
            body += conn.recv(content_len - len(body))

        # Routing Logic
        response_code = "200 OK"
        response_headers = {"Vary": "Accept", "Connection": "close"}
        response_body = b""

        if path == "/products/1":
            if method == "OPTIONS":
                response_code = "200 OK"
                response_headers["Allow"] = "GET, HEAD, PUT, PATCH, DELETE, OPTIONS"
                response_headers["Content-Length"] = "0"

            elif method in ("GET", "HEAD"):
                accepted = parse_q_values(headers.get("accept", "*/*"))
                payload, ctype = render_representation(DATABASE.get("1", {}), accepted)

                if not DATABASE.get("1"):
                    response_code = "404 Not Found"
                    response_body = b'{"error": "Resource deleted or not found"}'
                    response_headers["Content-Type"] = "application/json"
                elif payload is None:
                    response_code = "406 Not Acceptable"
                    response_body = b"Supported formats: application/json, text/html, text/plain"
                    response_headers["Content-Type"] = "text/plain"
                else:
                    response_code = "200 OK"
                    response_headers["Content-Type"] = ctype
                    response_body = payload if method == "GET" else b""

                response_headers["Content-Length"] = str(len(payload) if payload else len(response_body))

            elif method == "PUT":
                # Strict Idempotent Full Replacement
                try:
                    payload_json = json.loads(body.decode('utf-8'))
                    # Full replacement: All old keys erased except provided ones
                    DATABASE["1"] = {
                        "id": "1",
                        "name": payload_json.get("name"),
                        "price": payload_json.get("price")
                    }
                    response_code = "200 OK"
                    response_body = json.dumps(DATABASE["1"]).encode('utf-8')
                    response_headers["Content-Type"] = "application/json"
                    response_headers["Content-Length"] = str(len(response_body))
                except Exception as e:
                    response_code = "400 Bad Request"
                    response_body = str(e).encode('utf-8')
                    response_headers["Content-Length"] = str(len(response_body))

            elif method == "PATCH":
                # Non-Idempotent Relative Modification
                try:
                    patch_json = json.loads(body.decode('utf-8'))
                    # If action is 'increment_price', each call mutates state further
                    if "price_delta" in patch_json and "1" in DATABASE:
                        DATABASE["1"]["price"] += patch_json["price_delta"]
                    elif "name" in patch_json and "1" in DATABASE:
                        DATABASE["1"]["name"] = patch_json["name"]

                    response_code = "200 OK"
                    response_body = json.dumps(DATABASE.get("1", {})).encode('utf-8')
                    response_headers["Content-Type"] = "application/json"
                    response_headers["Content-Length"] = str(len(response_body))
                except Exception as e:
                    response_code = "400 Bad Request"
                    response_body = str(e).encode('utf-8')
                    response_headers["Content-Length"] = str(len(response_body))

            elif method == "DELETE":
                # Strict Idempotent Purge
                if "1" in DATABASE:
                    del DATABASE["1"]
                    response_code = "204 No Content"
                else:
                    response_code = "404 Not Found"
                response_headers["Content-Length"] = "0"

            else:
                response_code = "405 Method Not Allowed"
                response_headers["Allow"] = "GET, HEAD, PUT, PATCH, DELETE, OPTIONS"
                response_headers["Content-Length"] = "0"
        else:
            response_code = "404 Not Found"
            response_body = b"Not Found"
            response_headers["Content-Length"] = str(len(response_body))

        # Build raw wire response
        status_line = f"HTTP/1.1 {response_code}\r\n"
        hdr_lines = "".join(f"{k}: {v}\r\n" for k, v in response_headers.items())
        final_bytes = status_line.encode('ascii') + hdr_lines.encode('ascii') + b"\r\n" + response_body

        conn.sendall(final_bytes)
        conn.close()

if __name__ == "__main__":
    start_server()

```

---

#### 3 Ways to Inject Failure & Observe the Breakage

Run `python3 rest_server.py` in Terminal 1, then execute these sabotage operations in Terminal 2:

```
+-----------------------------------------------------------------------------------------+
| SOWING CHAOS: 3 SABOTAGE EXPERIMENTS                                                    |
+---+-----------------------------+-------------------------------+-----------------------+
| # | Sabotage Action             | Root Cause                    | What You Observe      |
+---+-----------------------------+-------------------------------+-----------------------+
| 1 | Partial Update via PUT      | Client treats PUT as PATCH;   | All unmentioned fields|
|   | Send:                       | sends only `{"name": "New"}`. | are erased/nullified; |
|   | `PUT {"name": "New"}`       | Server executes total record  | `"price"` becomes     |
|   |                             | replacement as per RFC spec.  | `None`.               |
+---+-----------------------------+-------------------------------+-----------------------+
| 2 | Non-Idempotent PATCH Replay | Automated network retry sends | Database price spikes |
|   | Send:                       | `PATCH {"price_delta": 10}`   | by $30 instead of $10;|
|   | 3 times consecutively.      | three times across timeouts.  | state corrupted.      |
+---+-----------------------------+-------------------------------+-----------------------+
| 3 | Unacceptable Media Type     | Client sends:                 | Server returns        |
|   | Negotiation                 | `Accept: audio/mp3, image/png`| `406 Not Acceptable`  |
|   |                             | with no matching server type. | rejecting processing. |
+---+-----------------------------+-------------------------------+-----------------------+

```

#### Executing the Sabotage Tests Live

**Experiment 1: The PUT Data Destruction Trap**

```bash
# Observe original state
curl -s http://127.0.0.1:8081/products/1
# {"id": "1", "name": "System Architecture Manual", "price": 45.0}

# Client attempts a partial update using PUT
curl -X PUT -H "Content-Type: application/json" -d '{"name": "Updated Manual Only"}' http://127.0.0.1:8081/products/1

# Inspect state after PUT
curl -s http://127.0.0.1:8081/products/1
# {"id": "1", "name": "Updated Manual Only", "price": null}
# -> Notice the price was completely wiped out because PUT is a complete overwrite.

```

**Experiment 2: The PATCH Duplicate Replay Hazard**

```bash
# Send relative mutation 3 times
curl -X PATCH -H "Content-Type: application/json" -d '{"price_delta": 10}' http://127.0.0.1:8081/products/1
curl -X PATCH -H "Content-Type: application/json" -d '{"price_delta": 10}' http://127.0.0.1:8081/products/1
curl -X PATCH -H "Content-Type: application/json" -d '{"price_delta": 10}' http://127.0.0.1:8081/products/1

# State has drifted by 30 units because PATCH is non-idempotent

```

**Experiment 3: Triggering 406 Not Acceptable via Content Negotiation**

```bash
curl -i -H "Accept: application/pdf, image/webp" http://127.0.0.1:8081/products/1
# Returns:
# HTTP/1.1 406 Not Acceptable
# Content-Type: text/plain
# Supported formats: application/json, text/html, text/plain

```

---

### Phase 5: The Invariant & The Capstone

#### The Non-Negotiable Invariant

> **HTTP Methods are explicit behavioral contracts for distributed state, not mere syntactic sugar.**
> `GET` guarantees zero state mutation (Safety), `PUT` and `DELETE` guarantee deterministic replayability ($f(f(x)) = f(x)$), while `POST` and `PATCH` convey state transitions that can never be safely replayed automatically across an unconfirmed network failure.

---

#### Day 4 Capstone Challenge

Using the `rest_server.py` provided above or writing your own minimal client/server script:

1. **Step 1:** Demonstrate Content Negotiation priority: Send a single `curl` command to `/products/1` with an `Accept` header containing `text/html;q=0.5, application/json;q=0.8, text/plain;q=0.2` and verify that the server returns `application/json`.
2. **Step 2:** Send another `curl` command with `text/html;q=0.9, application/json;q=0.8` and verify the server switches to returning raw HTML.
3. **Step 3:** Prove `DELETE` Idempotency: Send `DELETE /products/1` twice in succession. Record the status code of Call 1 (`204 No Content`) and Call 2 (`404 Not Found`), and explain why the underlying server database state is identical after both calls.
