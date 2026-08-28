# Week 7 - Day 4 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Structs, Memory Layout, Pointers (`*T`, `&x`), Value vs. Pointer Receivers, and Zero-Value Field Initialization.

---

## 🌐 Value vs. Pointer Receiver Architecture

```text
[ VALUE RECEIVER: func (p Packet) Display() ]
  Caller: packet ──► [ COPY OF STRUCT MEMORY ] ──► Method body operates on copy.
                     (Original struct unaffected by any changes)

[ POINTER RECEIVER: func (p *Packet) Verify() ]
  Caller: packet ──► [ MEMORY ADDRESS (&packet) ] ──► Method body dereferences address.
                     (Directly mutates original struct fields in-place)
```

---

## 1. Core Struct & Pointer Rules in Go

| Concept | Syntax / Mechanics | Engineering Advantage / Rule |
|---|---|---|
| **Zero-Value Allocation** | `p := Packet{}` initializes fields to defaults | Prevents uninitialized memory vulnerabilities (`0`, `""`, `false`). |
| **Empty Struct `struct{}`** | Occupies `0` bytes in RAM | Used for memory-efficient set implementations (`map[K]struct{}`) and signaling channels. |
| **Pointer Dereference `*p`** | Accesses underlying memory value | Automatic dereferencing syntax: `p.Field` automatically dereferences `(*p).Field`. |
| **No Pointer Arithmetic** | Pointer arithmetic (`ptr++`) is forbidden | Prevents buffer overflow & illegal memory access vulnerabilities. |
| **Pointer Receiver `(t *T)`** | Receives memory address of struct | Required when mutating state or avoiding expensive copies of large structs. |
