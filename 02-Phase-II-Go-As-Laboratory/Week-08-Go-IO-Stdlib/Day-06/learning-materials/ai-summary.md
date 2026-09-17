# Week 8 - Day 6 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Performance & Robustness: Micro-benchmarking (`*testing.B`), Memory Allocations (`-benchmem`), Go 1.24+ `b.Loop()`, and Coverage-Guided Native Fuzzing (`*testing.F`).

---

## 🌐 Benchmarking & Fuzzing Architecture

```text
[ MICRO-BENCHMARK: func BenchmarkXxx(b *testing.B) ]
  ├── Execution Loop: for b.Loop() { SUT() }  (Go 1.24+ idiomatic loop)
  ├── Metric 1: Execution Latency (ns/op)
  ├── Metric 2: Memory Footprint  (B/op)
  └── Metric 3: Heap Allocations  (allocs/op) via -benchmem
                                    
[ NATIVE FUZZ TEST: func FuzzXxx(f *testing.F) ]
  ├── Seed Corpus: f.Add("seed1"), f.Add("seed2")
  ├── Mutation Engine: Bit-flipping, splicing, dictionary mutation
  └── Invariant Target: f.Fuzz(func(t *testing.T, input string) {
          result := Slugify(input)
          assert(noUppercase(result))
          assert(noSpaces(result))
      })
```

---

## 1. Core Go Benchmark & Fuzzing Mechanics

| Tooling / Method | Invocation | Engineering Advantage |
|---|---|---|
| **`b.Loop()` (Go 1.24+)** | `for b.Loop() { ... }` | Replaces `for i := 0; i < b.N; i++` with safer timer management and zero manual reset overhead. |
| **Allocation Profiling** | `go test -bench=. -benchmem` | Discovers unwanted heap escapes and slice reallocation bottlenecks (`B/op`, `allocs/op`). |
| **Seed Corpus (`f.Add`)** | `f.Add(val1, val2)` | Seeds the mutation engine with known realistic inputs before random generation. |
| **Property-Based Invariants** | Invariant checking in `f.Fuzz` | Validates fundamental rules (e.g. no spaces, no uppercase) against hundreds of thousands of pseudorandom inputs. |
