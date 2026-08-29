aku# The One-Year Software Engineering Roadmap

## North Star

Become a **strong, backend-heavy full-stack software engineer with real systems, Linux, networking, database, infrastructure, security, concurrency, observability, and distributed-systems understanding**—while retaining TypeScript/React/Svelte as application-layer tools and using AI as an accelerator rather than a substitute for understanding.

**Primary language:** Go   
**Secondary language:** Rust (introduced after the foundations are strong)   
**Application language retained:** TypeScript   
**Core database:** PostgreSQL   
**Core cache:** Redis   
**Infrastructure:** Linux + Docker/Compose + Caddy + GitHub Actions   
**Later systems topics:** queues, WebSockets, observability, distributed systems, Rust

---

# 0. The Rules of the Year

## Rule 1 — One main road

Do not restart courses because a new technology looks exciting.

Curiosity is allowed; derailment is not.

The main road is:

`Linux → networking → HTTP → Go → SQL/PostgreSQL → backend architecture → security → testing → Docker/Linux deployment → Redis/concurrency → queues/realtime → observability → distributed systems → Rust`

TypeScript/React/Svelte remain side tools, not the curriculum center.

## Rule 2 — AI is allowed, but understanding is mandatory

For every meaningful feature:

1. **Design** — draw the architecture yourself.  
2. **Predict** — write what you think the implementation should do.  
3. **Generate** — ask AI for boilerplate or implementation help.  
4. **Audit** — read every relevant line.  
5. **Explain** — explain control flow, data flow, errors, security implications and dependencies.  
6. **Break** — deliberately create invalid input, race conditions, failures, timeouts and dependency outages.  
7. **Observe** — inspect logs, packets, SQL plans, metrics, process state, memory or traces.  
8. **Rebuild** — reproduce the important part without AI.  
9. **Record** — write what you learned.

For foundational exercises, sometimes use **AI-off blocks**.

## Rule 3 — Build before collecting

Every major topic must produce one of:

- a small experiment  
- a command-line tool  
- a protocol implementation  
- a test  
- a benchmark  
- a failure reproduction  
- a deployed service  
- a written engineering note

## Rule 4 — Learn the abstraction and the mechanism

For each technology ask:

- What problem does it solve?  
- What abstraction does it give me?  
- What mechanism is underneath?  
- What does it cost?  
- How does it fail?  
- How do I observe it?  
- When should I *not* use it?

## Rule 5 — Do not worship “low level” work

The goal is not to avoid abstractions forever. The goal is to understand enough underneath the abstraction to use it deliberately.

---

# 1. Weekly Operating System

Target: **40–50 hours/week**. More is optional; consistency matters more than heroic bursts.

Recommended week:

- **20–24 h:** primary study + implementation  
- **12–16 h:** project/build/debugging  
- **4–5 h:** DSA/interview fundamentals  
- **2–3 h:** reading/engineering notes  
- **2 h:** frontend maintenance / TypeScript / Svelte when desired  
- **1 h:** weekly review and planning

Do not force all 50 hours when your brain is fried. Sustained 40–50 hour weeks for many months beat 80-hour bursts followed by disappearance.

## Daily rhythm

### Block A — Learn  
60–120 minutes.

Read docs/book/lecture. Make notes.

### Block B — Mechanism  
90–150 minutes.

Use terminal, debugger, packet capture, SQL console, source code, benchmarks, etc.

### Block C — Build  
3–5 hours.

Implement a project feature.

### Block D — Break  
30–90 minutes.

Attack the thing you built.

### Block E — Record  
20–40 minutes.

Write:

- What I learned  
- What surprised me  
- What broke  
- Why it broke  
- What I would change  
- What I still cannot explain

---

# 2. Master Competency Map

By the end of the year you should have studied at least the following.

## Computer systems

- binary/hex basics  
- CPU, memory, storage, I/O  
- process  
- thread  
- scheduling  
- context switching  
- virtual memory  
- pages/page faults  
- stack/heap  
- address spaces  
- system calls  
- file descriptors  
- signals  
- pipes  
- environment variables  
- permissions  
- ownership  
- filesystem hierarchy  
- processes/services  
- logs  
- time and clocks  
- monotonic vs wall clocks

## Linux

- shell  
- Bash scripting  
- stdin/stdout/stderr  
- pipes/redirection  
- grep/sed/awk/find/xargs  
- permissions  
- users/groups  
- SSH  
- systemd  
- journalctl  
- processes  
- sockets  
- networking tools  
- disk/mounts  
- cron/system timers  
- package management  
- firewall basics  
- DNS configuration  
- TLS basics  
- service management

## Networking

- layered networking model  
- Ethernet/MAC  
- ARP  
- IPv4  
- subnetting  
- routing  
- NAT  
- ports  
- sockets  
- TCP  
- UDP  
- three-way handshake  
- sequence numbers/ACKs  
- retransmission  
- flow control  
- congestion control concept  
- DNS  
- recursive vs authoritative DNS  
- HTTP/1.1  
- HTTP/2  
- HTTP/3/QUIC overview  
- TLS  
- certificates  
- proxies  
- reverse proxies  
- load balancing  
- WebSockets  
- SSE  
- connection lifecycle  
- timeouts  
- keep-alive  
- retries  
- backpressure

## Go

- syntax  
- variables/constants  
- functions  
- pointers  
- structs  
- methods  
- interfaces  
- slices  
- arrays  
- maps  
- strings/runes/bytes  
- error handling  
- wrapping errors  
- packages/modules  
- generics basics  
- testing  
- benchmarks  
- fuzzing  
- profiling  
- standard library  
- `context`  
- `net`  
- `net/http`  
- `encoding/json`  
- `os`  
- `io`  
- `bufio`  
- `sync`  
- `sync/atomic`  
- goroutines  
- channels  
- mutexes  
- race detection  
- graceful shutdown  
- HTTP middleware  
- structured logging  
- configuration

## PostgreSQL/SQL

- DDL  
- DML  
- SELECT  
- joins  
- grouping  
- aggregates  
- subqueries  
- CTEs  
- recursive CTE awareness  
- window functions  
- constraints  
- foreign keys  
- cascades  
- uniqueness  
- check constraints  
- NULL semantics  
- transactions  
- ACID  
- MVCC  
- isolation levels  
- locks  
- deadlocks  
- indexes  
- B-trees  
- index selectivity  
- query planner  
- `EXPLAIN`  
- `EXPLAIN ANALYZE`  
- connection pooling  
- prepared statements  
- parameterized queries  
- migrations  
- backup/restore  
- WAL concept  
- replication concept  
- partitioning concept  
- full-text search awareness  
- JSONB awareness  
- PostGIS awareness

## Backend architecture

- request lifecycle  
- routing  
- handlers  
- services/use cases  
- repositories/data access  
- domain modeling  
- validation  
- error taxonomy  
- idempotency  
- pagination  
- filtering/sorting  
- transactions  
- optimistic/pessimistic concurrency  
- authentication  
- authorization  
- RBAC  
- object-level authorization  
- sessions  
- cookies  
- JWT concepts  
- token rotation/revocation concepts  
- rate limiting  
- caching  
- background jobs  
- webhooks  
- retries  
- dead-letter queues  
- graceful shutdown  
- health/readiness checks  
- API versioning  
- OpenAPI

## Security

- threat modeling  
- trust boundaries  
- attack surface  
- authentication vs authorization  
- password hashing  
- session security  
- CSRF  
- CORS  
- XSS awareness  
- SQL injection  
- SSRF  
- IDOR/BOLA  
- mass assignment/object-property authorization  
- insecure deserialization awareness  
- rate limiting  
- resource exhaustion  
- secrets management  
- TLS  
- secure headers  
- dependency hygiene  
- logging security events  
- principle of least privilege

## Testing

- unit tests  
- table-driven tests  
- integration tests  
- API tests  
- database tests  
- test fixtures  
- mocks vs fakes vs real dependencies  
- contract testing awareness  
- race testing  
- fuzz testing  
- benchmarks  
- load testing  
- failure testing  
- end-to-end testing awareness

## Infrastructure/DevOps

- Git  
- Git branching/merging/rebase  
- CI  
- CD  
- Docker images  
- Dockerfiles  
- layers  
- containers  
- volumes  
- networks  
- Compose  
- health checks  
- resource limits  
- secrets/config  
- reverse proxies  
- Caddy  
- TLS  
- DNS  
- Linux deployment  
- backups  
- restore drills  
- monitoring  
- logs  
- metrics  
- traces  
- OpenTelemetry  
- Prometheus/Grafana awareness

## Redis

- strings  
- hashes  
- lists  
- sets  
- sorted sets  
- TTL  
- atomic operations  
- pipelines  
- transactions  
- Lua awareness  
- pub/sub  
- cache-aside  
- cache invalidation  
- rate limiting  
- distributed locks and their failure modes  
- sessions

## Concurrency/distributed systems

- concurrency vs parallelism  
- goroutines  
- shared memory  
- message passing  
- mutexes  
- atomics  
- races  
- deadlocks  
- starvation  
- backpressure  
- queues  
- retries  
- exponential backoff  
- jitter  
- idempotency  
- at-least-once delivery  
- duplicate events  
- ordering  
- eventual consistency  
- CAP theorem  
- quorum concept  
- replication  
- leader/follower concept  
- failover  
- service discovery concept  
- distributed locks  
- leases  
- clock problems  
- logical clocks awareness  
- monotonic vs wall time  
- consistent hashing  
- sharding concepts

## System design

- requirements  
- constraints  
- capacity estimation  
- bottleneck analysis  
- latency vs throughput  
- availability  
- reliability  
- durability  
- consistency  
- caching  
- queues  
- databases  
- replicas  
- load balancers  
- partitioning/sharding  
- object storage  
- CDN awareness  
- observability  
- failure modes  
- graceful degradation  
- disaster recovery

## Rust

- cargo  
- ownership  
- borrowing  
- lifetimes  
- enums  
- pattern matching  
- `Option`  
- `Result`  
- traits  
- generics  
- iterators  
- smart pointers  
- `Box`  
- `Arc`  
- `Mutex`  
- `Send`  
- `Sync`  
- async/await  
- futures  
- memory layout  
- performance trade-offs  
- networking/service implementation

---

# 3. The 52-Week Curriculum

The week numbers are not prison bars. If a topic takes another week, move the calendar while keeping the sequence.

---

## PHASE I — COMPUTER, LINUX & NETWORKING  
### Weeks 1–6

---

## Week 1 — Computer fundamentals + shell

### Day 1  
- [x] CPU, RAM, storage, I/O, peripherals  
- [x] Binary/decimal/hexadecimal basics  
- [x] Learn what an instruction and machine code are conceptually  
- [x] Inspect your Linux machine with `lscpu`, `free`, `lsblk`, `lspci`

### Day 2  
- [x] Program vs process  
- [x] Executable vs source code  
- [x] Process address space  
- [x] PID/PPID  
- [x] Run and inspect processes with `ps`, `top`/`htop`

### Day 3  
- [x] Thread concept  
- [x] Process vs thread  
- [x] Context switching concept  
- [x] Scheduling concept  
- [x] Observe a multithreaded process

### Day 4  
- [x] Shell fundamentals  
- [x] stdin/stdout/stderr  
- [x] redirection  
- [x] pipes  
- [x] exit status  
- [x] `tee`, `xargs`

### Day 5  
- [x] Filesystem hierarchy  
- [x] permissions  
- [x] owner/group  
- [x] chmod/chown  
- [x] symbolic vs hard links

### Day 6  
- [x] Environment variables  
- [x] shell startup files  
- [x] PATH  
- [x] processes inherit environment  
- [x] write a Bash script

### Day 7 — Review  
- [x] Explain program → process → thread  
- [x] Build a CLI script combining pipes, files and environment variables  
- [x] Write a literature note: “What the OS is doing for my programs”

**Deliverable:** a shell toolbox + 1-page OS mental model.

---

## Week 2 — Memory, files, syscalls, time

### Day 1  
- [x] Stack vs heap concept  
- [x] dynamic allocation concept  
- [x] pointers/references concept

### Day 2  
- [x] Virtual memory  
- [x] virtual addresses  
- [x] pages  
- [x] page faults  
- [x] why every process thinks it has its own memory

### Day 3  
- [x] File descriptors  
- [x] stdin/stdout/stderr as descriptors  
- [x] files/sockets/pipes as OS resources

### Day 4  
- [x] System call concept  
- [x] user mode vs kernel mode  
- [x] `strace`  
- [x] trace a simple command

### Day 5  
- [x] open/read/write/close concept  
- [x] buffering  
- [x] filesystem metadata

### Day 6  
- [x] wall clock vs monotonic clock  
- [x] timeouts and why wall time is dangerous for durations  
- [x] experiment with timestamps and sleep/timing

### Day 7  
- [x] Rebuild mental model from scratch without notes  
- [x] Use `strace` to explain how a simple program starts and reads a file
- [x] CPU Traps, Binary Setup.

**Deliverable:** syscall/process/filesystem investigation report.

---

## Week 3 — Networking fundamentals

### Day 1  
- [x] Network layers  
- [x] Ethernet  
- [x] MAC addresses  
- [x] frames  
- [x] ARP concept

### Day 2  
- [x] IP addresses  
- [x] IPv4  
- [x] subnet masks/CIDR  
- [x] private vs public IPs  
- [x] default gateway

### Day 3  
- [x] Routing  
- [x] NAT concept  
- [x] ports  
- [x] sockets  
- [x] client/server

### Day 4  
- [x] UDP  
- [x] datagrams  
- [x] connectionless communication  
- [x] packet loss/reordering concept

### Day 5  
- [x] TCP  
- [x] three-way handshake  
- [x] sequence numbers  
- [x] acknowledgements  
- [x] retransmission

### Day 6  
- [x] TCP flow/congestion-control concepts  
- [x] keep-alive  
- [x] connection close  
- [x] TIME_WAIT concept

### Day 7  
- [x] Inspect sockets with `ss`  
- [x] Use `nc` to create a TCP conversation  
- [x] Document a packet’s journey from machine A to B

**Deliverable:** TCP/IP mental model + terminal lab notes.

---

## Week 4 — DNS + HTTP/1.1

### Day 1  
- [x] DNS purpose  
- [x] resolver  
- [x] recursive vs authoritative DNS  
- [x] A/AAAA/CNAME/TXT/NS/MX records

### Day 2  
- [x] Use `dig`  
- [x] inspect TTLs  
- [x] DNS caching  
- [x] trace DNS resolution conceptually

### Day 3  
- [x] HTTP request structure  
- [x] method  
- [x] target/path  
- [x] headers  
- [x] body  
- [x] response structure

### Day 4  
- [x] GET/POST/PUT/PATCH/DELETE/HEAD/OPTIONS  
- [x] idempotency  
- [x] safe methods  
- [x] content negotiation

### Day 5  
- [x] 1xx/2xx/3xx/4xx/5xx semantics  
- [x] cookies  
- [x] caching headers  
- [x] ETag / conditional requests

### Day 6  
- [x] `curl -v`  
- [x] raw HTTP with `nc`  
- [x] capture traffic with `tcpdump`  
- [x] compare HTTP/1.1 connection reuse

### Day 7  
- [x] Explain browser → DNS → TCP → HTTP completely  
- [ ] Build a tiny Bash HTTP client

**Deliverable:** “One HTTP request from typing a URL to receiving bytes.”

---

## Week 5 — TLS, proxies and modern HTTP

### Day 1  
- [x] cryptographic goals: confidentiality/integrity/authenticity  
- [x] hashing vs encryption vs signing

### Day 2  
- [x] symmetric encryption  
- [x] asymmetric cryptography  
- [x] key exchange concept

### Day 3  
- [x] TLS handshake concept  
- [x] certificates  
- [x] certificate chains  
- [x] hostname verification

### Day 4  
- [x] HTTPS with `curl -v`  
- [x] inspect certificates with OpenSSL tools

### Day 5  
- [x] Forward proxy vs reverse proxy  
- [x] load balancer concept  
- [x] TLS termination  
- [x] forwarded headers

### Day 6  
- [x] HTTP/2 concepts: framing, streams, multiplexing, header compression  
- [x] HTTP/3/QUIC overview

### Day 7  
- [x] Explain HTTPS without handwaving  
- [x] Sketch browser → CDN/proxy → server flow

**Deliverable:** [x] TLS + reverse-proxy note.

---

## Week 6 — Linux workflow, Git, debugging, profiling

### Day 1  
- [x] Git objects: commits/trees/blobs/refs  
- [x] branches  
- [x] merge  
- [x] rebase concept

### Day 2  
- [x] useful Git workflows  
- [x] bisect  
- [x] reflog  
- [x] recovery exercises

### Day 3  
- [x] debugger concepts  
- [x] breakpoints  
- [x] stack traces  
- [x] watch expressions

### Day 4  
- [x] profiling concept  
- [x] CPU vs memory vs I/O bottlenecks  
- [x] benchmark mindset

### Day 5  
- [x] MIT Missing Semester shell/tooling/debugging material  
- [x] reproduce a debugging workflow end-to-end

### Day 6  
- [x] shell aliases/functions  
- [x] tmux  
- [x] terminal productivity

### Day 7  
- [x] Phase I exam: explain the entire machine/network stack  
- [x] No notes for the first attempt

**Gate:** Do not move on until you can explain the complete request path and use Linux tools to investigate it.

---

# PHASE II — GO AS THE LABORATORY  
## Weeks 7–11

---

## Week 7 — Go fundamentals

### Day 1  
- [x] install Go  
- [x] `go env`  
- [x] modules  
- [x] `go run/build/test/fmt/vet`

### Day 2  
- [x] variables/constants  
- [x] functions  
- [x] control flow  
- [x] defer

### Day 3  
- [x] arrays  
- [x] slices  
- [x] maps  
- [x] strings/runes/bytes

### Day 4  
- [x] structs  
- [x] methods  
- [x] pointers  
- [x] zero values

### Day 5  
- [x] interfaces  
- [x] composition  
- [x] package boundaries

### Day 6  
- [ ] errors  
- [ ] wrapping  
- [ ] sentinel/custom errors

### Day 7  
- [ ] build a CLI task tracker  
- [ ] AI-off implementation first

---

## Week 8 — Go I/O and standard library

### Day 1  
- [ ] files  
- [ ] `io`  
- [ ] buffered I/O

### Day 2  
- [ ] JSON encode/decode  
- [ ] struct tags  
- [ ] validation boundaries

### Day 3  
- [ ] CLI flags  
- [ ] environment configuration

### Day 4  
- [ ] `context.Context`  
- [ ] cancellation  
- [ ] deadlines

### Day 5  
- [ ] testing package  
- [ ] table-driven tests

### Day 6  
- [ ] benchmarks  
- [ ] fuzzing

### Day 7  
- [ ] build a CLI log analyzer or file indexer

**Deliverable:** tested Go CLI tool.

---

## Week 9 — TCP with Go

### Day 1  
- [ ] `net.Listen`  
- [ ] sockets  
- [ ] accept loop

### Day 2  
- [ ] read/write bytes  
- [ ] framing problem  
- [ ] newline-delimited protocol

### Day 3  
- [ ] concurrent client handling  
- [ ] goroutines

### Day 4  
- [ ] timeouts  
- [ ] connection shutdown

### Day 5  
- [ ] build an echo server

### Day 6  
- [ ] build a tiny custom text protocol

### Day 7  
- [ ] intentionally break clients  
- [ ] inspect with `ss`/tcpdump

**Deliverable:** TCP server + protocol documentation.

---

## Week 10 — HTTP server without a framework

### Day 1  
- [ ] `net/http`  
- [ ] `http.Server`  
- [ ] handlers

### Day 2  
- [ ] request fields  
- [ ] query parameters  
- [ ] headers  
- [ ] body

### Day 3  
- [ ] JSON requests/responses  
- [ ] status codes

### Day 4  
- [ ] manual routing  
- [ ] path extraction

### Day 5  
- [ ] middleware concept  
- [ ] request logging

### Day 6  
- [ ] timeouts  
- [ ] graceful shutdown

### Day 7  
- [ ] build a tiny HTTP API only with stdlib

---

## Week 11 — Go architecture and concurrency foundations

### Day 1  
- [ ] packages  
- [ ] dependency direction  
- [ ] domain/application/infrastructure separation

### Day 2  
- [ ] goroutines  
- [ ] channels  
- [ ] blocking behavior

### Day 3  
- [ ] mutexes  
- [ ] RWMutex  
- [ ] atomics

### Day 4  
- [ ] WaitGroup  
- [ ] cancellation with context

### Day 5  
- [ ] race detector  
- [ ] reproduce a data race

### Day 6  
- [ ] graceful shutdown  
- [ ] worker lifecycle

### Day 7  
- [ ] mini exam: implement a concurrent TCP or HTTP service without AI

**Gate:** You can write and debug a small Go server from memory.

---

# PHASE III — POSTGRESQL UNMASKED  
## Weeks 12–17

---

## Week 12 — SQL foundations

### Day 1  
- [ ] schema/table/row/column  
- [ ] CREATE/ALTER/DROP

### Day 2  
- [ ] INSERT/UPDATE/DELETE  
- [ ] RETURNING

### Day 3  
- [ ] SELECT  
- [ ] WHERE  
- [ ] ORDER BY  
- [ ] LIMIT/OFFSET

### Day 4  
- [ ] NULL semantics  
- [ ] COALESCE  
- [ ] CASE

### Day 5  
- [ ] joins: INNER/LEFT  
- [ ] many-to-many

### Day 6  
- [ ] GROUP BY  
- [ ] HAVING  
- [ ] aggregates

### Day 7  
- [ ] design a normalized relational schema from scratch

---

## Week 13 — advanced SQL

### Day 1  
- [ ] subqueries  
- [ ] correlated subqueries

### Day 2  
- [ ] CTEs  
- [ ] recursive CTE awareness

### Day 3  
- [ ] window functions  
- [ ] ROW_NUMBER/RANK/LAG/LEAD

### Day 4  
- [ ] constraints  
- [ ] uniqueness  
- [ ] foreign keys  
- [ ] checks

### Day 5  
- [ ] cascading behavior  
- [ ] soft delete trade-offs

### Day 6  
- [ ] SQL injection  
- [ ] parameterized queries

### Day 7  
- [ ] 50-query SQL drill

---

## Week 14 — transactions and concurrency

### Day 1  
- [ ] ACID  
- [ ] transaction boundaries

### Day 2  
- [ ] BEGIN/COMMIT/ROLLBACK  
- [ ] savepoints

### Day 3  
- [ ] isolation levels  
- [ ] dirty/non-repeatable/phantom reads

### Day 4  
- [ ] MVCC mental model  
- [ ] snapshots/visibility

### Day 5  
- [ ] row locks  
- [ ] SELECT FOR UPDATE

### Day 6  
- [ ] deadlocks  
- [ ] reproduce a deadlock in two sessions

### Day 7  
- [ ] write a “concurrent ticket assignment” transaction correctly

---

## Week 15 — indexes and query planning

### Day 1  
- [ ] B-tree structure concept  
- [ ] index lookup

### Day 2  
- [ ] selectivity  
- [ ] composite indexes  
- [ ] order and prefix behavior

### Day 3  
- [ ] `EXPLAIN`  
- [ ] sequential scan/index scan

### Day 4  
- [ ] `EXPLAIN ANALYZE`  
- [ ] actual vs estimated rows

### Day 5  
- [ ] intentionally slow query  
- [ ] create an index  
- [ ] compare plan

### Day 6  
- [ ] sort/hash/aggregate nodes  
- [ ] join strategies

### Day 7  
- [ ] performance report showing before/after query plans

---

## Week 16 — PostgreSQL architecture

### Day 1  
- [ ] PostgreSQL process architecture  
- [ ] client/backend connection model

### Day 2  
- [ ] shared buffers concept  
- [ ] WAL concept

### Day 3  
- [ ] vacuum/autovacuum  
- [ ] bloat concept

### Day 4  
- [ ] connection pooling  
- [ ] connection limits

### Day 5  
- [ ] backup/restore  
- [ ] `pg_dump`/restore concepts

### Day 6  
- [ ] replication concept  
- [ ] primary/standby

### Day 7  
- [ ] JSONB/full-text/PostGIS awareness  
- [ ] know when relational is better than NoSQL

---

## Week 17 — Go + PostgreSQL

### Day 1  
- [ ] `database/sql` concepts  
- [ ] connection pools

### Day 2  
- [ ] pgx  
- [ ] native PostgreSQL integration

### Day 3  
- [ ] prepared/parameterized queries  
- [ ] scans

### Day 4  
- [ ] transactions from Go

### Day 5  
- [ ] migrations  
- [ ] seed data

### Day 6  
- [ ] integration tests against real PostgreSQL

### Day 7  
- [ ] build a small CRUD service with raw SQL

**Important:** for a modern PostgreSQL-only Go project, learn `pgx` rather than blindly following older `lib/pq` tutorials. `pgx` is actively developed and can also adapt to `database/sql`. citeturn574169search0turn574169search3

**Gate:** You can explain a query, transaction, lock and execution plan without hiding behind an ORM.

---

# PHASE IV — PRODUCTION BACKEND ENGINEERING  
## Weeks 18–25

---

## Week 18 — API design

### Day 1  
- [ ] resources  
- [ ] route design  
- [ ] HTTP semantics

### Day 2  
- [ ] request validation  
- [ ] error taxonomy  
- [ ] structured errors

### Day 3  
- [ ] pagination  
- [ ] cursor vs offset

### Day 4  
- [ ] filtering/sorting/search

### Day 5  
- [ ] idempotency  
- [ ] retries

### Day 6  
- [ ] API versioning  
- [ ] backward compatibility

### Day 7  
- [ ] OpenAPI design before implementation

---

## Week 19 — architecture

### Day 1  
- [ ] handler/service/repository separation

### Day 2  
- [ ] domain models vs persistence models vs DTOs

### Day 3  
- [ ] dependency inversion  
- [ ] interfaces where useful, not everywhere

### Day 4  
- [ ] transaction ownership  
- [ ] unit-of-work boundaries

### Day 5  
- [ ] configuration and environment management

### Day 6  
- [ ] structured logging  
- [ ] correlation/request IDs

### Day 7  
- [ ] refactor mini API into a deliberate architecture

---

## Week 20 — authentication

### Day 1  
- [ ] authentication vs authorization  
- [ ] identity lifecycle

### Day 2  
- [ ] password hashing  
- [ ] salts  
- [ ] adaptive hashing

### Day 3  
- [ ] sessions  
- [ ] secure cookies  
- [ ] session expiration/revocation

### Day 4  
- [ ] JWT structure  
- [ ] signing vs encryption  
- [ ] claims

### Day 5  
- [ ] access vs refresh tokens  
- [ ] rotation/revocation concepts

### Day 6  
- [ ] implement JWT verification as a learning exercise  
- [ ] learn why production should use a maintained library

### Day 7  
- [ ] threat-model your authentication flow

**Security note:** OWASP currently recommends Argon2id as the preferred password hashing choice, with bcrypt as a legacy/alternative option where appropriate. Do not treat “bcrypt everywhere” as the modern universal rule. citeturn319574search2

---

## Week 21 — authorization and API security

### Day 1  
- [ ] RBAC  
- [ ] permissions

### Day 2  
- [ ] object-level authorization  
- [ ] BOLA/IDOR

### Day 3  
- [ ] object-property authorization  
- [ ] mass assignment

### Day 4  
- [ ] function-level authorization

### Day 5  
- [ ] rate limiting  
- [ ] resource exhaustion

### Day 6  
- [ ] SSRF  
- [ ] unsafe external API consumption

### Day 7  
- [ ] security review against OWASP API Top 10

OWASP's 2023 API Top 10 emphasizes authorization failures, authentication failures, resource consumption, SSRF, security misconfiguration, inventory management and unsafe API consumption among other risks. citeturn222789search3turn222789search7

---

## Week 22 — validation, errors, resilience

### Day 1  
- [ ] validation at trust boundaries

### Day 2  
- [ ] domain invariants  
- [ ] database invariants

### Day 3  
- [ ] timeouts  
- [ ] cancellation

### Day 4  
- [ ] retries  
- [ ] exponential backoff  
- [ ] jitter

### Day 5  
- [ ] idempotency keys  
- [ ] duplicate request handling

### Day 6  
- [ ] partial failure  
- [ ] graceful degradation

### Day 7  
- [ ] failure injection exercises

---

## Week 23 — testing deeply

### Day 1  
- [ ] unit tests  
- [ ] table-driven style

### Day 2  
- [ ] integration tests  
- [ ] real PostgreSQL

### Day 3  
- [ ] HTTP API integration tests

### Day 4  
- [ ] mocks vs fakes vs real dependencies

### Day 5  
- [ ] race tests  
- [ ] flaky test diagnosis

### Day 6  
- [ ] fuzz tests  
- [ ] parser/input fuzzing

### Day 7  
- [ ] benchmark critical code paths

Go has native fuzz testing in the standard toolchain, which makes fuzzing worth incorporating into this curriculum rather than treating it as an exotic future topic. citeturn319574search3

---

## Week 24 — CampusCare Core begins

Build the backend you already understand as a domain, but now with Go + PostgreSQL and no Supabase.

### Day 1  
- [ ] requirements extraction  
- [ ] entities  
- [ ] invariants

### Day 2  
- [ ] schema design  
- [ ] constraints

### Day 3  
- [ ] ticket creation API

### Day 4  
- [ ] state-transition API

### Day 5  
- [ ] role authorization

### Day 6  
- [ ] audit logging

### Day 7  
- [ ] integration tests

---

## Week 25 — CampusCare Core hardening

### Day 1  
- [ ] pagination/filtering

### Day 2  
- [ ] SLA timestamps  
- [ ] monotonic duration reasoning

### Day 3  
- [ ] transactions  
- [ ] concurrency conflicts

### Day 4  
- [ ] rate limiting

### Day 5  
- [ ] security tests

### Day 6  
- [ ] load-test a representative endpoint

### Day 7  
- [ ] write the production architecture document

**Gate:** You can defend every architectural decision in CampusCare Core in an interview.

---

# PHASE V — DEPLOYMENT & DEVOPS  
## Weeks 26–31

---

## Week 26 — Docker fundamentals

### Day 1  
- [ ] what containers are  
- [ ] namespaces/cgroups concept

### Day 2  
- [ ] image layers  
- [ ] Dockerfile

### Day 3  
- [ ] build/run/inspect  
- [ ] logs

### Day 4  
- [ ] ports  
- [ ] networks

### Day 5  
- [ ] volumes  
- [ ] persistence

### Day 6  
- [ ] multi-stage Go builds  
- [ ] minimal runtime images

### Day 7  
- [ ] containerize CampusCare Core

Docker's current Compose documentation explicitly emphasizes services, networks, volumes, health checks and debugging the running stack; use those as actual learning objectives rather than memorizing YAML. citeturn222789search5turn222789search0

---

## Week 27 — Docker Compose and service orchestration

### Day 1  
- [ ] Compose services  
- [ ] environment configuration

### Day 2  
- [ ] PostgreSQL volume  
- [ ] initialization

### Day 3  
- [ ] health checks  
- [ ] startup race conditions

### Day 4  
- [ ] service networks  
- [ ] DNS between containers

### Day 5  
- [ ] resource limits  
- [ ] restart policies

### Day 6  
- [ ] logs  
- [ ] exec into containers

### Day 7  
- [ ] tear down/rebuild/restore the whole environment

---

## Week 28 — Linux deployment

### Day 1  
- [ ] SSH keys  
- [ ] sshd configuration

### Day 2  
- [ ] users/groups  
- [ ] least privilege

### Day 3  
- [ ] UFW/firewall concepts  
- [ ] exposed ports

### Day 4  
- [ ] systemd services  
- [ ] logs

### Day 5  
- [ ] DNS records  
- [ ] domain routing

### Day 6  
- [ ] backup/restore  
- [ ] disk space  
- [ ] log rotation

### Day 7  
- [ ] reproduce deployment from a clean server state

---

## Week 29 — Reverse proxy + HTTPS

### Day 1  
- [ ] reverse proxy architecture

### Day 2  
- [ ] Caddyfile  
- [ ] upstreams

### Day 3  
- [ ] HTTPS certificates  
- [ ] renewal

### Day 4  
- [ ] forwarded headers  
- [ ] client IP semantics

### Day 5  
- [ ] proxy timeouts  
- [ ] buffering

### Day 6  
- [ ] health endpoints  
- [ ] zero/low-downtime deployment concepts

### Day 7  
- [ ] publicly deploy the API safely

Caddy's current docs support straightforward reverse proxying and automatic HTTPS when a real domain is used, which makes it a good first reverse proxy for your learning server. citeturn222789search6turn222789search9

---

## Week 30 — CI/CD

### Day 1  
- [ ] CI concept  
- [ ] build/test pipeline

### Day 2  
- [ ] GitHub Actions basics

### Day 3  
- [ ] test/lint/vet

### Day 4  
- [ ] build container image

### Day 5  
- [ ] deployment strategy  
- [ ] rollback concept

### Day 6  
- [ ] secrets  
- [ ] environment separation

### Day 7  
- [ ] push-to-deploy pipeline

---

## Week 31 — backups, failure and operations

### Day 1  
- [ ] backup strategy  
- [ ] RPO/RTO

### Day 2  
- [ ] restore drill

### Day 3  
- [ ] disk exhaustion experiment

### Day 4  
- [ ] process crash/restart

### Day 5  
- [ ] database unavailable

### Day 6  
- [ ] reverse proxy unavailable

### Day 7  
- [ ] write an incident runbook

**Gate:** You can deploy, break, observe, recover and redeploy your service without a tutorial.

---

# PHASE VI — REDIS, CONCURRENCY & BACKGROUND PROCESSING  
## Weeks 32–37

---

## Week 32 — Redis fundamentals

### Day 1  
- [ ] strings  
- [ ] GET/SET

### Day 2  
- [ ] hashes  
- [ ] lists

### Day 3  
- [ ] sets  
- [ ] sorted sets

### Day 4  
- [ ] TTL  
- [ ] expiration

### Day 5  
- [ ] atomic operations  
- [ ] transactions

### Day 6  
- [ ] pub/sub

### Day 7  
- [ ] choose appropriate Redis data structures for five use cases

---

## Week 33 — caching

### Day 1  
- [ ] cache-aside

### Day 2  
- [ ] read-through/write-through concepts

### Day 3  
- [ ] invalidation  
- [ ] TTL trade-offs

### Day 4  
- [ ] stale data  
- [ ] cache stampede

### Day 5  
- [ ] negative caching

### Day 6  
- [ ] cache expensive PostgreSQL analytics

### Day 7  
- [ ] measure hit/miss ratio and latency

---

## Week 34 — rate limiting

### Day 1  
- [ ] fixed window  
- [ ] sliding window

### Day 2  
- [ ] token bucket  
- [ ] leaky bucket

### Day 3  
- [ ] local/in-memory limiting

### Day 4  
- [ ] distributed limiting with Redis

### Day 5  
- [ ] atomicity/race conditions

### Day 6  
- [ ] abuse scenarios

### Day 7  
- [ ] production-style Redis rate limiter

---

## Week 35 — Go concurrency deeply

### Day 1  
- [ ] concurrency vs parallelism

### Day 2  
- [ ] goroutine lifecycle

### Day 3  
- [ ] channel patterns  
- [ ] fan-in/fan-out

### Day 4  
- [ ] cancellation  
- [ ] contexts

### Day 5  
- [ ] Mutex/RWMutex

### Day 6  
- [ ] atomics  
- [ ] data races

### Day 7  
- [ ] race detector + benchmark comparison

---

## Week 36 — queues and workers

### Day 1  
- [ ] synchronous vs asynchronous work

### Day 2  
- [ ] channel-backed worker queue

### Day 3  
- [ ] job states  
- [ ] retries

### Day 4  
- [ ] exponential backoff/jitter

### Day 5  
- [ ] dead-letter queue concept

### Day 6  
- [ ] idempotent job processing

### Day 7  
- [ ] notification worker for CampusCare

---

## Week 37 — RabbitMQ/message-broker concepts

### Day 1  
- [ ] queues  
- [ ] exchanges/routing concepts

### Day 2  
- [ ] acknowledgements  
- [ ] redelivery

### Day 3  
- [ ] at-most-once/at-least-once

### Day 4  
- [ ] ordering  
- [ ] duplicate delivery

### Day 5  
- [ ] dead-lettering

### Day 6  
- [ ] consumer scaling

### Day 7  
- [ ] build a small event-driven workflow

---

# PHASE VII — REAL-TIME & OBSERVABILITY  
## Weeks 38–42

---

## Week 38 — WebSockets

### Day 1  
- [ ] persistent connection concept

### Day 2  
- [ ] upgrade handshake

### Day 3  
- [ ] read/write loops

### Day 4  
- [ ] ping/pong heartbeats

### Day 5  
- [ ] disconnect/reconnect

### Day 6  
- [ ] shared connection state

### Day 7  
- [ ] basic chat backend

---

## Week 39 — real-time correctness

### Day 1  
- [ ] concurrent map safety

### Day 2  
- [ ] broadcast architecture

### Day 3  
- [ ] slow consumer/backpressure

### Day 4  
- [ ] message ordering

### Day 5  
- [ ] offline/reconnection behavior

### Day 6  
- [ ] persistence/history

### Day 7  
- [ ] live CampusCare notifications

---

## Week 40 — metrics

### Day 1  
- [ ] what metrics answer

### Day 2  
- [ ] counters  
- [ ] gauges  
- [ ] histograms

### Day 3  
- [ ] request latency  
- [ ] error rate

### Day 4  
- [ ] throughput  
- [ ] saturation

### Day 5  
- [ ] Prometheus concepts

### Day 6  
- [ ] Grafana dashboards

### Day 7  
- [ ] instrument API and database metrics

---

## Week 41 — logs and traces

### Day 1  
- [ ] structured logging  
- [ ] correlation IDs

### Day 2  
- [ ] log levels  
- [ ] useful vs useless logs

### Day 3  
- [ ] distributed tracing concepts

### Day 4  
- [ ] spans  
- [ ] trace context

### Day 5  
- [ ] OpenTelemetry Go

### Day 6  
- [ ] trace HTTP → DB

### Day 7  
- [ ] diagnose a latency problem using telemetry

OpenTelemetry Go currently documents stable traces and metrics support, with logs still marked beta; it is appropriate to introduce observability before you reach distributed systems. citeturn319574search1

---

## Week 42 — load/failure testing

### Day 1  
- [ ] load testing concepts

### Day 2  
- [ ] throughput/latency percentiles  
- [ ] p50/p95/p99

### Day 3  
- [ ] connection limits

### Day 4  
- [ ] CPU/memory profiling

### Day 5  
- [ ] database bottleneck experiment

### Day 6  
- [ ] cache bottleneck experiment

### Day 7  
- [ ] write a performance postmortem

**Gate:** You can measure a backend rather than merely say it is “fast.”

---

# PHASE VIII — SYSTEM DESIGN & DISTRIBUTED SYSTEMS  
## Weeks 43–47

---

## Week 43 — system design fundamentals

### Day 1  
- [ ] requirements gathering  
- [ ] functional vs non-functional requirements

### Day 2  
- [ ] capacity estimation  
- [ ] requests/sec  
- [ ] storage growth

### Day 3  
- [ ] latency budget  
- [ ] throughput

### Day 4  
- [ ] availability  
- [ ] reliability  
- [ ] durability

### Day 5  
- [ ] bottleneck identification

### Day 6  
- [ ] architecture diagrams

### Day 7  
- [ ] design URL shortener

---

## Week 44 — scaling patterns

### Day 1  
- [ ] vertical scaling  
- [ ] horizontal scaling

### Day 2  
- [ ] load balancing

### Day 3  
- [ ] caching layers

### Day 4  
- [ ] read replicas

### Day 5  
- [ ] partitioning/sharding

### Day 6  
- [ ] consistent hashing

### Day 7  
- [ ] redesign one CampusCare subsystem for 10× load

---

## Week 45 — distributed consistency

### Day 1  
- [ ] CAP theorem

### Day 2  
- [ ] consistency models

### Day 3  
- [ ] eventual consistency

### Day 4  
- [ ] replication  
- [ ] leader/follower concept

### Day 5  
- [ ] quorum concepts

### Day 6  
- [ ] failover  
- [ ] split-brain awareness

### Day 7  
- [ ] design a replicated service on paper

---

## Week 46 — distributed coordination

### Day 1  
- [ ] distributed locks  
- [ ] leases

### Day 2  
- [ ] idempotency

### Day 3  
- [ ] deduplication

### Day 4  
- [ ] ordering  
- [ ] clocks

### Day 5  
- [ ] logical clocks awareness

### Day 6  
- [ ] retries and failure amplification

### Day 7  
- [ ] design a resilient payment/webhook workflow

---

## Week 47 — data-intensive architecture

### Day 1  
- [ ] queues vs streams

### Day 2  
- [ ] event-driven architecture

### Day 3  
- [ ] CQRS concept

### Day 4  
- [ ] event sourcing concept

### Day 5  
- [ ] search/indexing systems awareness

### Day 6  
- [ ] object storage/CDN awareness

### Day 7  
- [ ] DDIA chapter review + architecture essay

**Core book:** *Designing Data-Intensive Applications* (DDIA).

---

# PHASE IX — RUST / SYSTEMS PARADIGM  
## Weeks 48–52

Rust comes late intentionally. The objective is not merely to learn another syntax; it is to let Rust force a second mental model onto foundations you already understand.

---

## Week 48 — Rust foundations

### Day 1  
- [ ] toolchain  
- [ ] cargo  
- [ ] crates

### Day 2  
- [ ] variables  
- [ ] mutability  
- [ ] functions

### Day 3  
- [ ] ownership

### Day 4  
- [ ] borrowing  
- [ ] references

### Day 5  
- [ ] slices  
- [ ] structs

### Day 6  
- [ ] enums  
- [ ] pattern matching

### Day 7  
- [ ] Result/Option

The Rust Book's ownership chapter explicitly connects ownership/borrowing to memory-safety guarantees without a garbage collector; study it deeply rather than rushing past it. citeturn506308search6turn506308search8

---

## Week 49 — Rust memory and type system

### Day 1  
- [ ] stack vs heap in Rust

### Day 2  
- [ ] moves vs copies

### Day 3  
- [ ] lifetimes

### Day 4  
- [ ] traits

### Day 5  
- [ ] generics

### Day 6  
- [ ] iterators  
- [ ] closures

### Day 7  
- [ ] rewrite a Go CLI in Rust

---

## Week 50 — Rust concurrency

### Day 1  
- [ ] threads

### Day 2  
- [ ] Arc  
- [ ] Mutex

### Day 3  
- [ ] Send  
- [ ] Sync

### Day 4  
- [ ] channels

### Day 5  
- [ ] async/await  
- [ ] futures

### Day 6  
- [ ] async runtime concepts

### Day 7  
- [ ] compare Go goroutines/channels with Rust threads/async

Rust's current standard documentation describes `async` functions/blocks as producing futures that run when awaited; use the official Rust Book and async materials rather than memorizing framework syntax. citeturn506308search1turn506308search7

---

## Week 51 — Rust networking

### Day 1  
- [ ] TCP service

### Day 2  
- [ ] HTTP service

### Day 3  
- [ ] JSON

### Day 4  
- [ ] concurrency

### Day 5  
- [ ] graceful shutdown

### Day 6  
- [ ] benchmark against Go implementation

### Day 7  
- [ ] memory/performance comparison

---

## Week 52 — Final capstone + engineering audit

### Day 1  
- [ ] architecture review  
- [ ] threat model

### Day 2  
- [ ] database review  
- [ ] indexes/queries

### Day 3  
- [ ] concurrency review

### Day 4  
- [ ] deployment review

### Day 5  
- [ ] observability review

### Day 6  
- [ ] disaster/restore drill  
- [ ] performance test

### Day 7  
- [ ] final technical write-up  
- [ ] record what you know  
- [ ] record what you still do not know  
- [ ] write the next 12-month roadmap

---

# 4. Capstone Architecture

Build one serious backend rather than twenty toy APIs.

## Recommended capstone

### CampusCare Core — Go Production Backend

Components:

```text  
Clients  
  │  
  │ HTTPS  
  ▼  
Caddy  
  │  
  ▼  
Go API  
  ├── Auth  
  ├── RBAC  
  ├── Ticket domain  
  ├── SLA engine  
  ├── Audit log  
  ├── Rate limiting  
  ├── WebSocket notifications  
  ├── Background workers  
  └── OpenTelemetry  
       │  
       ├──────── PostgreSQL  
       │  
       └──────── Redis

Worker  
  └── email/notification jobs

Observability  
  ├── metrics  
  ├── logs  
  └── traces

Infrastructure  
  ├── Docker Compose  
  ├── Debian  
  ├── Caddy  
  └── GitHub Actions  
```

## Required capstone properties

- [ ] Raw SQL  
- [ ] PostgreSQL constraints  
- [ ] Transactions  
- [ ] migrations  
- [ ] indexes  
- [ ] authentication  
- [ ] authorization  
- [ ] object-level authorization  
- [ ] secure password storage  
- [ ] rate limiting  
- [ ] caching  
- [ ] background jobs  
- [ ] retries  
- [ ] idempotency  
- [ ] WebSockets  
- [ ] structured logging  
- [ ] metrics  
- [ ] traces  
- [ ] unit tests  
- [ ] integration tests  
- [ ] fuzz tests for at least one parser/boundary  
- [ ] CI  
- [ ] Docker  
- [ ] deployed Linux server  
- [ ] HTTPS  
- [ ] backup/restore drill  
- [ ] load test  
- [ ] incident/runbook documentation  
- [ ] architecture diagram  
- [ ] security/threat model

---

# 5. Side Projects — Use Them as Laboratories

Do not turn each item into a three-month product.

## Mini Project 1 — TCP Echo/Chat Server

Teaches:

- sockets  
- TCP  
- concurrency  
- protocol framing

## Mini Project 2 — URL Shortener

Teaches:

- database modeling  
- uniqueness  
- indexing  
- redirect semantics  
- analytics

## Mini Project 3 — Weather API Wrapper

Teaches:

- external APIs  
- timeout  
- retries  
- caching  
- rate limits

## Mini Project 4 — Job Board API

Teaches:

- relationships  
- filtering  
- authorization  
- pagination

## Mini Project 5 — Webhook Processor

Teaches:

- signatures  
- idempotency  
- queues  
- retries  
- dead-letter behavior

## Mini Project 6 — Chat/WebSocket Server

Teaches:

- persistent connections  
- state  
- concurrency  
- backpressure

## Mini Project 7 — Redis-like toy cache

Teaches:

- TCP protocol  
- in-memory storage  
- expiration  
- concurrency

## Final Project — CampusCare Core

Teaches everything together.

---

# 6. DSA Track — 4–5 Hours Every Week

Do not spend six months “preparing” for DSA. Keep it running in parallel.

## Months 1–2

- arrays  
- strings  
- hash maps  
- two pointers  
- sliding window  
- stack/queue  
- binary search

## Months 3–4

- linked lists  
- trees  
- BST  
- heap/priority queue  
- recursion/backtracking

## Months 5–6

- BFS  
- DFS  
- graph representations  
- topological sort  
- DSU

## Months 7–8

- greedy  
- intervals  
- prefix sums  
- monotonic stack/queue

## Months 9–10

- dynamic programming fundamentals  
- 1D/2D DP

## Months 11–12

- mixed interview sets  
- timed problems  
- review patterns

The goal is not competitive-programming celebrity status. The goal is to comfortably solve common interview problems and reason algorithmically.

---

# 7. TypeScript / Frontend Maintenance Lane

You are **not abandoning frontend**.

But it stops consuming the curriculum.

Use about 2 hours/week or occasional weekends.

Maintain:

- TypeScript  
- React  
- Next.js  
- Svelte  
- browser APIs  
- accessibility  
- state management  
- performance  
- testing

Do not spend weeks tuning Tailwind layouts during this year.

Your future frontend work can be passion/product work. Svelte belongs there.

---

# 8. System Design Reading Track

Start lightly around Week 20 and intensify from Week 43.

For every design problem, write:

1. requirements  
2. assumptions  
3. scale estimate  
4. API  
5. data model  
6. architecture  
7. bottlenecks  
8. consistency requirements  
9. failure modes  
10. observability  
11. security  
12. alternatives/trade-offs

Practice:

- URL shortener  
- chat  
- notification system  
- file upload service  
- job queue  
- rate limiter  
- news feed  
- ticketing platform  
- payment/webhook system  
- ride dispatch concept  
- metrics platform

---

# 9. Reading Curriculum

## Primary / highest priority

### 1. MIT Missing Semester

Use it early for shell/tooling/debugging/Git/packaging. The 2026 edition explicitly includes shell, command-line environment, debugging/profiling, Git, packaging, agentic coding, and code quality. citeturn574169search4turn574169search6

### 2. Go official material

- A Tour of Go  
- Go language specification as a reference  
- package documentation  
- standard library source  
- Effective Go for idioms, with the awareness that it is an older document and does not cover later language/ecosystem developments comprehensively. citeturn506308search0

### 3. PostgreSQL documentation

Use the official docs for SQL, transactions, indexes, explain, MVCC, configuration and administration.

### 4. MDN HTTP

Use HTTP guides to understand message structure, semantics, HTTP/2 and HTTP/3 evolution. citeturn506308search2turn506308search4

### 5. OWASP

- API Security Top 10  
- Password Storage Cheat Sheet  
- Authentication guidance  
- threat-model/security material

### 6. DDIA

Read progressively after you have built enough systems to recognize the problems it describes.

---

# 10. Books — Order Matters

## Read deeply this year

1. **The Missing Semester** material  
2. **The Go Programming Language** / official Go learning material  
3. **Designing Data-Intensive Applications**  
4. **A Philosophy of Software Design**  
5. **The Pragmatic Programmer**  
6. **Release It!**  
7. **Refactoring**  
8. **The Rust Programming Language** during Phase IX

## Read selectively / later

- Code Complete  
- Clean Architecture  
- Domain-Driven Design  
- The Mythical Man-Month  
- Designing Distributed Systems  
- Software Engineering at Google  
- Working Effectively with Legacy Code

Do not attempt to “finish every engineering book.” Books are reference layers around actual engineering work.

---

# 11. Engineering Articles / Sources

Keep these as a rotating reading shelf.

## Organizations

- Cloudflare Engineering  
- Stripe Engineering  
- GitHub Engineering  
- Netflix Tech Blog  
- Uber Engineering  
- Meta Engineering  
- Amazon Builders' Library  
- Google SRE  
- Oxide Computer

## Independent voices

- Martin Fowler  
- Julia Evans  
- Charity Majors  
- Dan Luu  
- The Pragmatic Engineer  
- Bartosz Ciechanowski

## Curated repositories

- Professional Programming  
- Pante's Reading List  
- Awesome lists  
- software-papers repositories

---

# 12. Engineering Principles to Study Through Practice

Do not memorize these as slogans.

Learn where each helps and where it becomes dogma.

- [ ] KISS  
- [ ] YAGNI  
- [ ] DRY  
- [ ] SOLID  
- [ ] composition over inheritance  
- [ ] separation of concerns  
- [ ] dependency inversion  
- [ ] single responsibility  
- [ ] fail fast  
- [ ] defensive programming  
- [ ] immutability  
- [ ] pure functions  
- [ ] idempotency  
- [ ] least privilege  
- [ ] explicit over magical  
- [ ] boring technology  
- [ ] observability as a feature  
- [ ] design for failure  
- [ ] optimize with evidence  
- [ ] make code easy to delete  
- [ ] simple systems before distributed systems

---

# 13. Notes System for Obsidian

Create these folders:

```text  
00-Inbox  
01-Concepts  
02-Mechanisms  
03-Protocols  
04-Projects  
05-Failures  
06-Architecture  
07-Books  
08-Papers  
09-Engineering  
10-Interview  
11-Glossary  
12-Retrospectives  
```

## Concept note template

```markdown  
# <Concept>

## Definition

## Why it exists

## Mental model

## Mechanism

## Example

## Failure modes

## Security implications

## Performance implications

## Related concepts

## What I can now explain

## What I still don't understand

## Experiment

## Sources  
```

## Failure note template

```markdown  
# Failure: <short name>

## Symptom

## Expected

## Actual

## Environment

## Reproduction

## Root cause

## Why I initially misunderstood it

## Fix

## Prevention

## Lesson

## Related concepts  
```

## Architecture note template

```markdown  
# <System>

## Requirements

## Constraints

## Actors

## APIs

## Data model

## Request flow

## State transitions

## Failure modes

## Security boundaries

## Scaling bottlenecks

## Observability

## Trade-offs

## Alternatives rejected

## Open questions  
```

---

# 14. Weekly Review

Every seventh day:

- [ ] Explain this week's core concepts without notes  
- [ ] Rebuild at least one thing without AI  
- [ ] Break one previous project  
- [ ] Fix one bug without asking AI first  
- [ ] Update permanent notes  
- [ ] Write one architecture diagram  
- [ ] Solve 3–5 DSA problems  
- [ ] Write 3 things that remain fuzzy  
- [ ] Decide what to revisit next week

## Every 4 weeks

Ask:

> “Could I teach everything I studied this month to another engineer?”

If no, identify the weak layer and revisit it.

---

# 15. Monthly Gates

## End of Month 1

You can explain Linux processes, memory, files, sockets, TCP/IP, DNS and HTTP at a conceptual level and investigate them with terminal tools.

## End of Month 2

You can write a small Go TCP and HTTP server without a framework.

## End of Month 3

You can design schemas, write serious SQL, reason about transactions and read basic execution plans.

## End of Month 4

You can design and implement a standalone Go API with authentication/authorization and tests.

## End of Month 5

You can containerize and deploy the system to Linux with HTTPS and CI.

## End of Month 6

You understand caching, concurrency, rate limiting and background processing well enough to implement them.

## End of Month 7

You can build a stateful WebSocket service and observe a backend in production-like conditions.

## End of Month 8

You can discuss load, latency, throughput, bottlenecks, queues and failure modes in system design terms.

## End of Month 9

You understand distributed-system trade-offs rather than merely naming Kafka/Redis/Kubernetes.

## End of Month 10

You can design a serious system from requirements to deployment and failure analysis.

## End of Month 11

Rust ownership/borrowing is becoming an actual mental model, not syntax trivia.

## End of Month 12

You have one substantial deployed backend, several supporting labs, strong written engineering notes, and enough breadth/depth to interview for backend-heavy software roles.

---

# 16. The “Intermediate Engineer” Standard

At the end of the year, do not measure yourself by the number of technologies listed on your CV.

Measure yourself by whether you can do these things:

### A. Design

Given a product requirement, you can derive:

- domain model  
- API  
- schema  
- invariants  
- architecture  
- failure modes  
- security boundaries  
- observability plan

### B. Implement

You can independently implement the majority of a backend without needing AI to decide what the architecture should be.

### C. Debug

When something breaks, you know how to investigate instead of randomly changing code.

You can inspect:

- logs  
- stack traces  
- CPU/memory  
- socket state  
- HTTP traces  
- SQL plans  
- database locks  
- network traffic  
- metrics  
- distributed traces

### D. Operate

You can deploy, monitor, update, rollback, backup and restore a service.

### E. Reason

You can answer:

- Why this database?  
- Why this transaction boundary?  
- Why this index?  
- Why this cache?  
- Why this queue?  
- Why this retry policy?  
- Why is this operation idempotent?  
- Where is the trust boundary?  
- What happens when the dependency is down?  
- What happens when the same event arrives twice?  
- What happens when two users update the same object simultaneously?

### F. Communicate

You can write a clear design document and explain trade-offs.

That is the level that makes “AI-assisted coding” powerful instead of dangerous.

---

# 17. What NOT to Chase This Year

Do not make these the center of the year:

- Kubernetes internals  
- Kafka internals  
- service mesh  
- five cloud providers  
- ten frontend frameworks  
- every Go web framework  
- every ORM  
- GraphQL just because it exists  
- microservices just because they sound senior  
- premature event sourcing  
- premature CQRS  
- advanced Rust before systems fundamentals

Know what they are. Use them only when the problem demands them.

The most valuable sentence you can learn is:

> **“A simpler system is sufficient here.”**

---

# 18. Career Track — Run in Parallel from Month 6

## Portfolio

### Project 1  
Go + PostgreSQL URL shortener, deployed.

### Project 2  
Job board API with authentication/authorization.

### Project 3  
CampusCare Core — serious flagship backend.

### Project 4  
Realtime chat/notification service.

Do not create ten mediocre GitHub repositories.

Create a few systems you can defend under interrogation.

## Resume evidence

Show:

- architecture diagrams  
- performance measurements  
- security considerations  
- tests  
- deployment  
- CI  
- incident/debugging write-ups  
- trade-off decisions

“Built API” is weak.

“Designed and deployed a Go/PostgreSQL service; diagnosed query bottlenecks with EXPLAIN ANALYZE; added indexes; implemented idempotent background processing; instrumented request traces; deployed via Docker/Caddy/CI” is evidence.

## Interviews

From Month 6 onward:

- 3–5 DSA problems/week  
- 1 backend design/week  
- 1 SQL exercise/week  
- 1 behavioral story/week

From Month 9 onward:

- timed mock interviews  
- system design  
- debugging interviews  
- SQL interviews  
- networking/HTTP questions  
- Linux fundamentals

---

# 19. AI Workflow — Production Version

## Before AI

Write:

```text  
Goal:  
Constraints:  
Inputs:  
Outputs:  
State:  
Failure modes:  
Security concerns:  
Performance concerns:  
```

## During AI use

Ask for:

- explanation  
- alternatives  
- tests  
- edge cases  
- failure modes  
- references to official docs

Do not merely ask:

> “Build this entire application.”

## After AI

Do these manually:

- review diffs  
- run tests  
- inspect generated SQL  
- inspect network behavior  
- mutate input  
- trigger failures  
- benchmark  
- delete/reimplement critical pieces

## Golden question

At any time you should be able to ask:

> **“What assumption is this code making?”**

That question will save you from a frightening amount of AI-generated nonsense.

---

# 20. The Lifelong Road After Year 1

Year 1 is not “become an engineer.”

It is “build the foundation that lets lifelong engineering work.”

After that, branch according to interest:

## Systems

- operating systems  
- C  
- kernel concepts  
- filesystems  
- compilers  
- runtimes  
- networking stacks

## Distributed systems

- consensus  
- Raft  
- replication  
- distributed transactions  
- stream processing  
- large-scale storage

## Backend

- API platforms  
- databases  
- queues  
- performance  
- cloud architecture

## Infrastructure

- Kubernetes  
- Terraform  
- cloud networking  
- SRE  
- disaster recovery

## Rust

- Tokio  
- Axum  
- async systems  
- storage engines  
- networking  
- high-performance services

## AI infrastructure

- inference serving  
- model gateways  
- vector retrieval  
- job scheduling  
- GPU orchestration  
- observability  
- data pipelines  
- model evaluation infrastructure

## Frontend/product

- React  
- Svelte  
- browser internals  
- Web Workers  
- WebGPU  
- Web Audio  
- performance engineering

You can eventually travel through all of these.

You simply cannot master them all simultaneously.

---

# 21. The North Star

Do not become a person who knows 80 technologies.

Become the person who can look at a system and ask:

> What is the actual problem?

> What are the constraints?

> What abstraction are we using?

> What is underneath it?

> Where can it fail?

> How will we know it failed?

> How do we recover?

> What does this cost?

> Is there a simpler solution?

Then build it.

Then break it.

Then understand why it broke.

Then build it better.

That loop is the curriculum.

---

# Current-Year Resource Corrections

This roadmap deliberately modernizes several items in older versions of the plan:

1. **PostgreSQL driver:** use `pgx` for new PostgreSQL-focused Go work rather than blindly following old `lib/pq` tutorials. `pgx` provides a native driver and can adapt to `database/sql`. citeturn574169search0  
2. **Router:** start with Go's standard `net/http`; when a router becomes useful, prefer a currently maintained lightweight option such as `chi` rather than building a curriculum around older Gorilla examples.  
3. **Password storage:** learn bcrypt, but treat Argon2id as the modern preferred choice according to OWASP. citeturn319574search2  
4. _******_**JWT:**_******_ implement verification logic once for learning, then use a maintained library in a real application. Never write production cryptography simply to prove you can.  
5. _******_**Observability:**_******_ add metrics, logs and traces before distributed systems. OpenTelemetry Go is mature enough to make this a practical part of the core backend curriculum. citeturn319574search1  
6. _******_**HTTP:**_******_ understand HTTP/1.1 semantics first, then HTTP/2 and HTTP/3/QUIC. The semantics remain the core mental model while the wire representation evolves. citeturn506308search2turn506308search4  
7. _******_**Docker:**_******_ learn images, containers, networks, volumes and health checks as mechanisms—not YAML syntax. citeturn222789search0turn506308search3

---

**# One-Line Daily Log**

At the end of every day, write:

`Today I learned _**__**__**_; I proved it by _**__**_**____; I broke _**__**__**_; the failure taught me _**__**_**____; tomorrow I will _____ .`

Keep doing that for a year.

That document will become a record of the engineer you actually became.