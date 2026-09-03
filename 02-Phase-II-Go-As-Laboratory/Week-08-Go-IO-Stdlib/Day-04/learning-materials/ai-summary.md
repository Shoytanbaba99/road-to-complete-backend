# Week 8 - Day 4 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go `context.Context`: Tree-based Cancellation, `context.WithTimeout` / `context.WithCancel`, Deadline Exceeded signals, and propagating abort signals through long-running operations.

---

## 🌐 Context Cancellation & Timeout Propagation Architecture

```text
[ ROOT CONTEXT: context.Background() ]
                 │
                 ▼
[ CHILD CONTEXT: context.WithTimeout(parent, duration) ]
  ├── Returns: ctx, cancel()
  ├── Spawns internal timer / deadline
  │
  ├──► Channel: <-ctx.Done()  (Closed when timer fires or cancel() invoked)
  └──► Error:   ctx.Err()      (context.DeadlineExceeded or context.Canceled)
                 │
                 ▼
[ BOUNDARY WORKER: ExportReport(ctx, ...) ]
  Loop: Encode Task ──► Check ctx.Err() ──► Abort and Rollback if non-nil
```

---

## 1. Core Go Context Mechanisms

| Context Function / Method | Signature | Purpose & Engineering Lifecycle |
|---|---|---|
| **`context.Background()`** | `func Background() Context` | Root context for main entrypoints, requests, or CLI lifecycles. |
| **`context.WithTimeout()`** | `func WithTimeout(parent, timeout) (Context, CancelFunc)` | Derives child context with automatic deadline; always `defer cancel()` to prevent goroutine/timer leaks. |
| **`context.WithCancel()`** | `func WithCancel(parent) (Context, CancelFunc)` | Derives child context cancelled manually on signal or error. |
| **`ctx.Err()`** | `func Err() error` | Returns `nil` while active; returns `context.Canceled` or `context.DeadlineExceeded` when done. |
| **`errors.Is(err, context.DeadlineExceeded)`** | Error inspection | Differentiates timeout failures from business or disk I/O errors. |
