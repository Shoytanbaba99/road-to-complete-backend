# Week 8 - Day 2 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go JSON Processing: Stream Encoding/Decoding (`json.NewEncoder`, `json.NewDecoder`), In-Memory Operations (`json.Marshal`, `json.Unmarshal`), Struct Tags (`json:"field_name,omitempty"`), and Domain Validation Boundaries.

---

## 🌐 Stream vs. In-Memory JSON Architecture

```text
[ IN-MEMORY JSON: json.Marshal / json.Unmarshal ]
  RAM Buffer []byte ──► json.Unmarshal() ──► Memory Struct Pointer &v
  (Requires holding entire JSON payload in memory before parsing)

[ STREAM JSON: json.NewEncoder / json.NewDecoder ]
  io.Reader (os.File / HTTP Body) ──► json.NewDecoder().Decode(&v) ──► Struct
  io.Writer (os.File / HTTP Response) ◄── json.NewEncoder().Encode(v) ◄── Struct
  (Streams data in chunks; ideal for files and network sockets)
```

---

## 1. Core Go JSON & Validation Mechanics

| Pattern / Utility | Syntax Example | Engineering Advantage |
|---|---|---|
| **Struct Tags** | `Id string json:"id"` | Maps lowercase JSON keys to exported Go struct fields. |
| **Stream Decoder** | `json.NewDecoder(file).Decode(&tasks)` | Reads directly from `io.Reader` streams without loading full file into RAM. |
| **Stream Encoder** | `json.NewEncoder(file).SetIndent("", "  ")` | Formats and streams pretty-printed JSON directly to `io.Writer`. |
| **Validation Boundary** | `(t *Task) Validate() error` | Enforces domain invariants immediately after JSON unmarshaling/decoding. |
