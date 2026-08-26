# Week 7 - Day 2 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Syntax, Functions, Control Flow (`switch`/`if`), Named Returns, and `defer` Resource Cleanup Mechanics.

---

## 🌐 Defer LIFO & Resource Lifecycle Architecture

```text
[ FUNCTION EXECUTION ]
  Acquire Resource ──► defer res.Close() ──► Process Business Logic ──► Return
                                                     │
                                                     ▼
                                          [ DEFER STACK (LIFO) ]
                                          1. Defer #2 (Audit Logger)
                                          2. Defer #1 (res.Close())
                                                     │
                                                     ▼
                                          [ EXECUTED AT FUNCTION EXIT ]
```

---

## 1. Core Go Control Flow & Function Patterns

| Feature / Construct | Go Syntax Pattern | Engineering Advantage |
|---|---|---|
| **`defer` Keyword** | `defer res.Close()` | Guarantees resource cleanup (files, sockets, DB connections) regardless of return path or error. |
| **LIFO Defer Order** | Multiple `defer` calls execute in Last-In, First-Out order | Mirrors stack unwind order (closes inner resources before outer resources). |
| **Named Returns** | `func Foo() (val int, err error)` | Pre-declares zero-valued return variables that can be inspected/mutated by `defer`. |
| **Expressionless `switch`** | `switch { case err != nil: ... }` | Replaces complex nested `if-else` chains with readable predicate branches. |
