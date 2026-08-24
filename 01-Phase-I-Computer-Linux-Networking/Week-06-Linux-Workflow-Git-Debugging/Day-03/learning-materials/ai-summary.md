# Week 6 - Day 3 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Debugger Mechanics: Breakpoints (`INT 3` / `0xCC` Trap & Opcode Replacement), DWARF Debug Symbols (Line Number & Variable Address Maps), Stack Frame Unwinding (`RBP` Linked-List Traversal), and `SIGTRAP` Signals.

---

## 🌐 Debugger Breakpoint & Trap Architecture

```text
[ NORMAL BINARY EXECUTION ]
  0x401000: mov rax, 1
  0x401005: add rax, rbx   <-- Breakpoint Target
  0x401008: ret

[ DEBUGGER SETS BREAKPOINT ]
  1. Save original byte '0x48' (add rax, rbx) into Debugger Hashmap
  2. Overwrite 0x401005 with INT 3 Trap Byte '0xCC'

[ RUNTIME EXECUTION & TRAP ]
  CPU executes 0x401000 ──► hits 0x401005 (0xCC) ──► Triggers SIGTRAP
  │
  ▼
[ DEBUGGER WAKES UP ]
  1. Pauses target process state
  2. Restores original byte '0x48' at 0x401005
  3. Decrements Instruction Pointer (RIP) back by 1 byte to 0x401005
  4. Allows user inspection (registers, stack frames, variables via DWARF map)
  5. Single-steps original instruction and re-installs 0xCC trap
```

---

## 1. Core Debugging Architecture Mechanisms

| Component / Mechanism | How It Works Under the Hood | Purpose in Debugging |
|---|---|---|
| **`INT 3` / `0xCC` Trap** | Single-byte software interrupt instruction | Pauses process execution and sends `SIGTRAP` to debugger. |
| **DWARF Debug Info** | Structured metadata table compiled into binaries (`-g`) | Maps memory addresses to source file lines and variable names. |
| **Stack Frame Unwinding** | Follows `RBP` frame pointers up the call stack | Reconstructs the linked-list sequence of active function calls. |
| **Watch Expressions** | CPU Hardware Breakpoint Registers (`DR0`-`DR3`) | Triggers pause when a specific memory location is read/written. |
