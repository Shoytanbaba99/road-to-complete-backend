# Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural summary of Day 1 computer architecture, hardware mechanisms, memory protection, and cache line performance characteristics.

---

## 🌐 The Grand Architecture: System Flow

```text
[ Physical Voltage ] (High = 1, Low = 0)
         │
         ▼
[ Binary (Base-2) ]  ──(Grouped by 4 bits)──► [ Hexadecimal (Base-16) ] ──► [ ASCII Text (#, A, B) ]
         │
         ▼
[ ISA / Opcode Dictionary ] (x86 CISC vs ARM RISC)
         │
         ▼
[ CPU Instruction Cycle ] (Fetch ➔ Decode ➔ Execute ➔ Store)
         │
         ▼
[ Memory Hierarchy & Protection ]
   ├── CPU Registers (Few KB - Nanoseconds)
   ├── L1/L2 Cache (MBs - 64-byte Cache Lines)
   ├── RAM (GBs - 4 KB Virtual Pages managed by MMU)
   └── Storage (SSDs / HDDs / MMIO Peripherals)
```

---

## 1. Stored-Program Concept
The **Von Neumann Architecture** establishes that both **instructions (code) and data (variables) share a single unified memory space (RAM)**. The CPU fetches both instructions and operational data from the same physical memory layer.

---

## 2. Number Systems & Encoding Pipeline
- Transistors operate on **High Voltage (1)** and **Low Voltage (0)** states ($\rightarrow$ **Binary Base-2**).
- **Hexadecimal (Base-16)** groups binary into 4-bit nibbles. Exactly 2 hex characters represent 1 byte ($8$ bits).
- **ASCII** maps byte values to human-readable characters:
  - `0x23` in Hex = `35` in Decimal = `#` in ASCII
  - `0x20` in Hex = `32` in Decimal = `[Space]` in ASCII
- **Custom Hex-Dumper (`hexdump.py`):** Reads binary byte streams and outputs Hex offsets (`0x00`, `0x10` = 16 bytes), raw hex byte codes, and ASCII representations.

---

## 3. CPU Architecture & Execution Cycle
The CPU executes an endless hardware loop:
1. **Fetch:** Retrieve the binary byte instruction from RAM using the Program Counter.
2. **Decode:** Map the binary opcode via the **Instruction Set Architecture (ISA)** dictionary (x86 CISC vs ARM RISC).
3. **Execute:** Run arithmetic/logic operations in the ALU.
4. **Store:** Write output back to registers or RAM.

---

## 4. The Memory Hierarchy & Storage Mechanics
- **CPU Registers:** Sub-nanosecond latency, tiny capacity (kilobytes).
- **CPU Cache (L1/L2/L3):** Ultra-fast SRAM integrated on the CPU silicon die operating in **64-byte Cache Lines**.
- **RAM:** One-dimensional byte-addressable array with uniform access latency.
- **Secondary Storage:** Persistent block devices (4 KB blocks). HDDs use spinning magnetic platters, SSDs use NAND flash floating-gate transistors, CDs use laser pits and lands.
- **Memory-Mapped I/O (MMIO):** Peripheral device registers (GPU, USB controllers) are mapped into CPU memory address ranges (`lspci -v` inspecting PCIe ranges like `0xfc200000`).

---

## 5. Significance of the 64-Byte Cache Line (Performance & Traps)

### What Happens When a Pointer Traverses Across Cache Lines?

1. **Spatial Locality & Cache Hits (Fast):**
   When a CPU reads a single byte from RAM, it does not fetch 1 byte alone; it fetches the entire surrounding **64-byte Cache Line** into L1 cache. Sequential array reads stay within the cache line, resulting in **L1 Cache Hits (~1-2 clock cycles / ~1 ns)**.

2. **Cache Misses & Pointer Hopping (Slow):**
   If a pointer jumps randomly across memory non-sequentially (e.g., chasing pointer links across memory scattered outside 64-byte boundaries), every access triggers a **Cache Miss**. The CPU must stall and wait **50–100 nanoseconds** to fetch a new 64-byte line from main RAM. This introduces a **50x-100x latency penalty**.

3. **False Sharing in Concurrency:**
   If two CPU cores concurrently modify two completely separate variables that happen to reside on the *same* 64-byte cache line, the CPU cache coherence protocol (MESI) constantly invalidates and bounces the cache line between cores, severely degrading multithreaded performance.

---

## 6. Memory Protection: Virtual Memory & MMU
- Operating systems assign each process an isolated **Virtual Address Space** partitioned into **4 KB Pages (4,096 bytes)**.
- The **Memory Management Unit (MMU)** hardware translates virtual addresses into physical RAM slot addresses using page tables.
- **Segmentation Fault (`SIGSEGV`):** If a pointer accesses an unmapped or unauthorized virtual page address, the MMU generates a hardware trap interrupt, halting CPU execution so Linux can terminate the process.
