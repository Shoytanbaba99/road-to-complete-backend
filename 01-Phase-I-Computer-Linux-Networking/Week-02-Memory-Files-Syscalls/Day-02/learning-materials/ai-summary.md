# Week 2 - Day 2 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Virtual Memory, Memory Management Unit (MMU) Page Translation, 4 KB Pages, Page Tables, Page Faults, Demand Paging, and Heap Allocator mechanics (`brk`/`sbrk`).

---

## 🌐 Virtual Memory Translation & Page Fault Architecture

```text
[ VIRTUAL ADDRESS (Issued by CPU / Program) ]
├── Virtual Page Number (VPN)  ──► [ PAGE TABLE (In RAM / MMU Cache) ]
└── Page Offset (0 - 4095)                                 │
                                                           ▼ (Translation)
                                            [ PHYSICAL RAM FRAME ]
                                            ├── Physical Address
                                            └── Page Fault Trap (If unmapped ➔ Demand Paging)
```

---

## 1. Virtual Memory & Process Isolation
- **The Problem:** Programs cannot write directly to physical RAM chips. If two processes used raw RAM addresses, they would overwrite each other's state and collapse the OS.
- **The Abstraction:** Every process receives an isolated **Virtual Address Space** (e.g. $0 \rightarrow 2^{64}-1$ on 64-bit systems). The process believes it has contiguous memory starting at address `0x00000000`.

---

## 2. 4 KB Pages & Page Tables
- **Byte Granularity vs Page Granularity:** Tracking every individual byte in RAM would require gigabytes of page table overhead.
- **4 KB Pages:** Memory is divided into fixed-size **4 KB chunks (4096 bytes)**.
- **MMU & Page Tables:** The Memory Management Unit (MMU) hardware chip uses multi-level **Page Tables** in RAM to map Virtual Page Numbers (VPN) to Physical Frame Numbers (PFN).

---

## 3. Page Faults & Demand Paging
- **Lazy Memory Allocation (Demand Paging):** When a process calls `malloc()`, the OS kernel does NOT allocate physical RAM immediately. It merely reserves virtual page addresses.
- **Page Fault Trap:** When the CPU attempts to read/write to an unmapped virtual address, the MMU hardware triggers a **Page Fault Interrupt**.
- **OS Kernel Handling:** The Linux kernel catches the interrupt, allocates a physical 4 KB RAM frame, updates the process's page table, and resumes execution seamlessly.

---

## 4. Heap Allocator Mechanics (`mini_malloc.c` & `sbrk`)
- User-space allocators (`malloc`/`free`) request memory from the kernel by adjusting the **Program Break** pointer using `sbrk()` or requesting anonymous page mappings via `mmap()`.
- **Free Lists:** Allocators maintain an internal linked list of memory blocks (`struct block_meta`). When `free(ptr)` is called, the block is marked as `free = 1` for future reuse without making expensive kernel system calls.
