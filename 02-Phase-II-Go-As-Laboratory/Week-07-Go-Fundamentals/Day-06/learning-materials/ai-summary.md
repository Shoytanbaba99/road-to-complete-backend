# Week 7 - Day 6 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Error Handling: Errors as Values (`error` interface), Sentinel Errors, Custom Error Structs, Error Wrapping (`fmt.Errorf("%w")`), `errors.Is()` / `errors.As()`, and `errors.Join()` (Go 1.20+).

---

## 🌐 Go Error Chain Architecture

```text
[ WRAPPED ERROR CHAIN ]
  fmt.Errorf("login failed: %w", err)
  │
  └──► Outer Error: "login failed: rate limit exceeded..."
        └──► Inner Error (%w): &RateLimitError{WaitSeconds: 60}

[ ERROR INSPECTION UTILITIES ]
  errors.Is(err, ErrSentinel) ──► Checks if ErrSentinel exists anywhere in the chain.
  errors.As(err, &customErr)  ──► Unwraps and assigns target type pointer if found in chain.
```

---

## 1. Core Go Error Handling Patterns

| Error Pattern | Implementation Syntax | Engineering Use Case |
|---|---|---|
| **Sentinel Errors** | `var ErrNotFound = errors.New("not found")` | Static package errors expected by callers for control flow. |
| **Custom Struct Errors** | `type MyErr struct { Code int }` | Rich errors carrying structured metadata (status codes, retry delays). |
| **Error Wrapping** | `fmt.Errorf("context: %w", err)` | Adds operational context while keeping underlying error inspectable. |
| **`errors.Is(err, target)`** | Traverses wrapped error chain | Compares against sentinel errors without string parsing. |
| **`errors.As(err, &target)`** | Unwraps and type-asserts | Extracts custom error struct data safely from wrapped chain. |
| **`errors.Join(err1, err2)`**| Combines multiple errors (Go 1.20+) | Aggregates multi-field validation errors or concurrent task failures. |
