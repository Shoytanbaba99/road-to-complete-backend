# Week 8 - Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go I/O: `io.Reader`, `io.Writer`, `os.File` descriptors, System Call reduction, `bufio.Reader`, `bufio.Writer` (`Flush()`), and `bufio.Scanner`.

---

## 🌐 Go I/O Stream & Buffer Architecture

```text
[ USER SPACE APPLICATION ]
  bufio.Writer (RAM Buffer e.g. 4KB/8KB)
  ├── 1. WriteString("task_1|...") ──► Appended to RAM Buffer (0 Syscalls)
  ├── 2. WriteString("task_2|...") ──► Appended to RAM Buffer (0 Syscalls)
  └── 3. Flush()                  ──► Batched Syscall Write (1 sys_write)
                                            │
                                            ▼
[ KERNEL SPACE / FILE SYSTEM ]
  os.File (File Descriptor) ──────► Disk / Persistent Block Device
```

---

## 1. Core Go I/O Interfaces & Utilities

| Utility / Interface | Function Signature / Method | Purpose / Performance Behavior |
|---|---|---|
| **`io.Reader`** | `Read(p []byte) (n int, err error)` | Reads up to `len(p)` bytes into byte slice `p`. Returns `io.EOF` at end. |
| **`io.Writer`** | `Write(p []byte) (n int, err error)` | Writes `len(p)` bytes from `p` to stream. |
| **`bufio.Writer`** | `bufio.NewWriter(w)` | Accumulates writes in RAM; reduces `sys_write` context switches. **Must call `Flush()`!** |
| **`bufio.Scanner`**| `scanner.Scan()`, `scanner.Text()` | Reads line-by-line or token-by-token from streams like `os.File` or `os.Stdin`. |
| **`os.Create` / `os.Open`** | Returns `*os.File` | Opens OS file descriptors (`O_RDONLY`, `O_WRONLY|O_CREATE|O_TRUNC`). |
