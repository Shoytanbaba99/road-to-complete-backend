# Week 6 - Day 4 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Profiling Concepts: CPU vs. Memory vs. I/O Bottlenecks, Memory Latency Hierarchy, Linux `time` Metrics (`real`, `user`, `sys`), and Parallel I/O Concurrency Benchmarking.

---

## 🌐 Memory & Storage Latency Hierarchy

```text
[ SPEED & COST ]                                              [ CAPACITY ]
  Fastest / Tiny   ┌──────────────────────────────────┐        Bytes / KB
                   │ CPU Registers (< 1 cycle)         │
                   ├──────────────────────────────────┤
                   │ L1 Cache (1–2 cycles, ~1 ns)     │
                   ├──────────────────────────────────┤
                   │ L2/L3 Cache (10–40 cycles, ~4 ns) │
                   ├──────────────────────────────────┤
                   │ System RAM (100–200 cycles, ~60ns)│        Gigabytes
                   ├──────────────────────────────────┤
                   │ NVMe / SSD (10–100 microseconds) │
                   ├──────────────────────────────────┤
                   │ Network / Remote API (1–100 ms)  │        Terabytes+
  Slowest / Cheap  └──────────────────────────────────┘
```

---

## 1. Linux `time` Command Metrics

| Metric | What It Measures | Engineering Interpretation |
|---|---|---|
| **`real`** | Total wall-clock time elapsed from start to finish | Real time experienced by the end-user. |
| **`user`** | CPU time spent executing user-space code | Measures pure application CPU computation. |
| **`sys`** | CPU time spent executing kernel-space system calls | Measures I/O, socket operations, page faults, and context switches. |

---

## 2. Bottleneck Classification

- **CPU-Bound (`user` time ≈ `real` time):** Application maxes out CPU cores with computation (e.g. cryptography, JSON parsing, sorting).
- **I/O-Bound (`real` >> `user` + `sys`):** Process spends most of its time blocked waiting for external I/O (network packets, disk read/write).
- **Memory-Bound:** Application starves CPU caches, causing frequent RAM page faults or memory thrashing.
