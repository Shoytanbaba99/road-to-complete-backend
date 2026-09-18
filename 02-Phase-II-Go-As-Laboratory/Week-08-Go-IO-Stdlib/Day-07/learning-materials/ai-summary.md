# Week 8 - Day 7 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of the Week 8 Capstone: `logscan` CLI Log Analyzer combining File I/O (`os.Open`), Buffered Streaming (`bufio.Scanner`), Context Cancellation (`context.WithTimeout`), Struct Tagged JSON Output (`encoding/json`), CLI Flags (`flag`), Micro-benchmarks (`*testing.B`), and Fuzzing (`*testing.F`).

---

## 🌐 Capstone Architecture Diagram

```text
[ CLI INVOCATION: ./logscan -file access.log -timeout 5 ]
                          │
                          ▼
[ FLAG PARSER: flag.String("file"), flag.Int("timeout") ]
                          │
                          ▼
[ CONTEXT ROOT: context.WithTimeout(Background, timeout) ]
                          │
                          ▼
[ FILE STREAM: os.Open(filepath) -> io.Reader ]
                          │
                          ▼
[ STREAM PROCESSOR: ProcessStream(ctx, reader) ]
  ├── bufio.Scanner line-by-line streaming
  ├── Context check: ctx.Err() cancellation boundary
  └── ParseLine: Method, URL, StatusCode, DurationMS
                          │
                          ▼
[ STATS ACCUMULATOR: Stats.AddEntry ]
  ├── TotalRequests increment
  ├── StatusCode frequency map
  └── Rolling duration & max latency tracking
                          │
                          ▼
[ JSON STREAM SERIALIZER: json.NewEncoder(os.Stdout).Encode(stats) ]
```

---

## 1. Core Integration Matrix

| Subsystem | Component | Implementation Detail |
|---|---|---|
| **Streaming I/O** | `bufio.Scanner` | Line-by-line parsing avoids loading multi-gigabyte access log files into memory. |
| **Context Safety** | `ctx.Err()` | Early cancellation check per line prevents CPU spin if timeout triggers. |
| **CLI Flags** | `flag.StringVar`, `flag.IntVar` | Structured options with automated usage message on missing arguments. |
| **Serialization** | `json.NewEncoder` | Pretty-printed JSON stream written directly to standard output (`os.Stdout`). |
| **Quality Verification** | Unit, Benchmark & Fuzz | Table-driven line parsing, sub-microsecond latency benchmarks, and fuzz mutation resilience. |
