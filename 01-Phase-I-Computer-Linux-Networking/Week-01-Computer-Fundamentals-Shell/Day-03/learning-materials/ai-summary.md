# Day 3 Architectural Overview & Technical Reference

> **Scope:** High-level architectural summary of Threads vs. Processes, shared memory address spaces, thread-local stacks, POSIX threads (`pthread`), race conditions, mutex locks, CPU scheduling, and SIMD performance characteristics.

---

## 🌐 Process vs. Thread Memory Layout

```text
[ PROCESS MEMORY ADDRESS SPACE ]
├── Text Segment (Read-only machine code)  ── Shared by all threads
├── Data / BSS Segment (Global variables)  ── Shared by all threads
├── Heap Segment (Dynamic malloc memory)  ── Shared by all threads
│
├── Thread 1 Stack (Local variables & call frames)  ── Private to Thread 1
├── Thread 2 Stack (Local variables & call frames)  ── Private to Thread 2
└── Thread N Stack (Local variables & call frames)  ── Private to Thread N
```

---

## 1. Process vs. Thread
- **Process:** An isolated execution environment created by `fork()`. Possesses its own independent virtual address space, file descriptor table, and PID. Inter-process communication (IPC) requires explicit OS mechanisms (pipes, sockets, shared memory).
- **Thread:** A lightweight unit of execution within a process created by `pthread_create()`. Threads share the **same virtual address space** (Code, Heap, Globals, File Descriptors), but each thread gets its own private **Stack Pointer and Registers**.

---

## 2. Shared Memory & Race Conditions
- Because all threads read/write to the same global data and heap memory, concurrent access without synchronization introduces **Race Conditions**.
- **Data Race:** Occurs when two threads access the same memory location concurrently, at least one access is a write, and there is no synchronization.
- **Mutex (`pthread_mutex_t`):** Mutual Exclusion lock used to enforce atomic access to critical sections. Threads must acquire the lock before modifying shared state and release it afterward.

---

## 3. Context Switching & Scheduling Overhead
- **Process Context Switch:** Heavyweight. Requires swapping Virtual Memory Page Tables (updating the `CR3` register on x86), flushing the Translation Lookaside Buffer (TLB), and swapping CPU registers.
- **Thread Context Switch:** Lightweight. Keeps the page table intact (no TLB flush) and only swaps CPU registers and stack pointers.
- **Overhead:** Spawning hundreds of threads or contending heavily for mutex locks introduces context-switching overhead and lock contention, degrading execution speed.

---

## 4. Benchmark Engineering: Custom Multithreaded C vs. GNU `wc -l`

### Day 3 Capstone Benchmark Results (1 GB File / 8,390,835 Newlines):
- **Custom Multithreaded C (`chunk_counter.c` - 4 Threads + Mutex):** `4.976s`
- **GNU `wc -l` (Single-Threaded SIMD Stream):** `1.235s`

### Why GNU `wc -l` Outperforms Multithreaded C:
1. **SIMD / Vectorization:** GNU `wc` uses Single Instruction Multiple Data (SIMD) CPU instructions (AVX2/AVX-512). Instead of checking 1 byte at a time in a loop, SIMD inspects **32 to 64 bytes simultaneously in a single CPU clock cycle**.
2. **Zero Thread & Lock Overhead:** GNU `wc` runs as a single-threaded stream processor. It eliminates `pthread_create()` overhead, context switching, and mutex lock contention.
3. **Memory Bus Saturation:** Modern CPUs can saturate RAM bandwidth on a single core when processing SIMD vector registers without thread synchronization delays.
