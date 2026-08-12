# My Take & Synthesis

> **Goal:** Write down your own mental model, key insights, and personal understanding after studying the raw materials for the day.

## 🧠 Core Mental Model

The fundamental concept is the **Stored-Program Concept**: to execute multiple tasks, instructions and data must be saved in a unified memory space. The instruction, operation, and result are all represented and stored in memory. For that, we have a storage hierarchy from core to periphery: `CPU registers -> Cache -> RAM -> Secondary Storage (SSD/HDD) -> Archival Storage (Datacenter)`.

### Storage Hierarchy & Mechanics

- **CPU:** Only has a few kilobytes of registers. This is where processing and assembly execution occur.
- **Cache:** Microscopic, ultra-fast memory built into the CPU to quickly retrieve data and instructions without hitting RAM.
- **RAM:** A 1D, array-like storage system where any address can be accessed with uniform latency. The CPU writes and reads data from RAM, which is then persisted to or fetched from storage.
- **Secondary Storage:** HDDs use spinning magnetic platters with a read/write head. SSDs use NAND flash memory cells to trap electrons without moving parts. Archival tape memory is used for massive, long-term cold storage.
- **Memory-Mapped I/O (MMIO):** Instead of writing only to physical RAM, the motherboard maps peripheral hardware registers into the CPU's memory address space. The CPU writes to a memory address, and the hardware routes the signal directly to the GPU or I/O controller.
- **Blocks & Filesystems:** The CPU reads storage in blocks (e.g., 4096 bytes) rather than bit-by-bit. The filesystem is an OS software abstraction built on top of these raw block devices.

### Number Systems: Binary, Hexadecimal, Decimal

- **Binary (Base-2):** Computers operate on high ($1$) or low ($0$) voltage states.
- **Decimal (Base-10):** Human-readable power-of-10 math.
- **Hexadecimal (Base-16):** Power-of-16 representation that conveniently maps 4 binary bits (a nibble) to 1 hex digit (`0-F`), making raw binary readable for humans.

### ISA & Machine Code

- **Instruction Set Architecture (ISA):** The hardware dictionary of binary opcodes physically wired into the CPU circuit to recognize commands (`ADD`, `SUB`, `MUL`, `DIV`).
- **Machine Code:** The raw binary stream ($0$s and $1$s) executed by the CPU.
- **RISC vs CISC:** Two CPU architectural philosophies. RISC (e.g., ARM) uses simplified, uniform instructions optimized for power efficiency. CISC (e.g., x86) uses complex instructions that hardware decoders break down into internal micro-ops.
- **Endianness:** Little-Endian stores the least significant byte at the lowest memory address.

---

## 💡 Key Takeaways

1. **Hierarchy:** `CPU Registers -> Cache -> RAM -> Secondary Storage` or `CPU -> MMIO Peripherals`.
2. **Architectures:** RISC (ARM) vs CISC (x86) with distinct ISAs.
3. **MMU Protection:** The MMU translates virtual addresses to physical addresses and prevents illegal memory accesses across boundaries.

---

## 🔬 Practical Lab Findings

Inspecting PCI devices using `lspci -v`:

```text
28:00.3 USB controller: Advanced Micro Devices, Inc. [AMD] Matisse USB 3.0 Host Controller (prog-if 30 [XHCI])
        Subsystem: Micro-Star International Co., Ltd. [MSI] Device 7b89
        Flags: bus master, fast devsel, latency 0, IRQ 48, IOMMU group 18
        Memory at fc200000 (64-bit, non-prefetchable) [size=1M]
        Kernel driver in use: xhci_hcd
        Kernel modules: xhci_pci
```

You can see the exact physical memory address (`Memory at fc200000`) that the CPU uses via MMIO to communicate directly with the USB hardware controller!

## Questions: 
- How does Cd,DVD and HDD store information through their perennial hand?
