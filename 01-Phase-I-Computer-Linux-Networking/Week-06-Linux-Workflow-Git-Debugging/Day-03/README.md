# Week 6 - Day 3: Debugger Concepts — Breakpoints, DWARF Symbols, Stack Traces & Program Execution Control

---

## 📋 Objectives
- [x] Debugger Mechanics: Breakpoint opcode swapping (`INT 3` / `0xCC` trap)
- [x] DWARF Symbol Tables & Source Code ↔ Binary Address Mapping
- [x] Stack Frame Unwinding & Linked-List Frame Pointer Traversal
- [x] Watch Expressions & CPU Hardware Breakpoint Registers (`DR0`-`DR3`)
- [x] Study debugging theory & concepts (skipped raw Python `ptrace` tracer implementation)

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model of breakpoint trap swapping (`0xCC`), DWARF tables, and stack frame unwinding. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming `INT 3` trap execution and DWARF memory mapping architecture. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory on debugger internals, DWARF format specifications, and stack frame layouts. |
| 📁 [**`learning-materials/debug_symbol.c`**](learning-materials/debug_symbol.c) | **Reference C Code:** Source program used for studying DWARF symbol compilation and stack trace generation. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Debugger Internals, Breakpoint Opcode Swapping & DWARF Symbol Tables]]` in `Engineers-Playbook/02 Permanent/`
