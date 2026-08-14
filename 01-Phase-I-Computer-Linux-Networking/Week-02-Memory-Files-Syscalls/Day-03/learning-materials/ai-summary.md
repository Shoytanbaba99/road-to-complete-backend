# Week 2 - Day 3 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of File Descriptors, Per-Process FD Tables, System-Wide Open File Description Tables, VFS Inode Tables, Standard I/O Streams (`0 STDIN`, `1 STDOUT`, `2 STDERR`), `dup2()` Stream Redirection, and Kernel Pipe IPC Mechanics.

---

## 🌐 File Descriptor Lookup Hierarchy

```text
[ PROCESS TASK STRUCT ]
  └── File Descriptor Table (fd_array)
      ├── FD 0 (STDIN)  ──► [ Open File Description ] ──► [ Terminal / Device ]
      ├── FD 1 (STDOUT) ──► [ Open File Description ] ──► [ File / Pipe ]
      ├── FD 2 (STDERR) ──► [ Open File Description ] ──► [ Log File ]
      └── FD 3 (Custom) ──► [ Open File Description (Offset 0x2A) ] ──► [ VFS Inode (Disk Block) ]
```

---

## 1. File Descriptor Table Architecture
- **Per-Process FD Table:** Small integer index table (`0, 1, 2, 3...`) mapped inside the kernel task control block of each process.
- **Open File Description Table:** System-wide kernel table tracking file offset position, status flags (`O_RDONLY`, `O_WRONLY`), and access modes.
- **Inode Table:** VFS filesystem metadata engine mapping file blocks on physical storage.

---

## 2. Standard Streams & Redirection (`dup2`)
- **POSIX Convention:** `0` (STDIN), `1` (STDOUT), `2` (STDERR).
- **`dup2(oldfd, newfd)` System Call:** Atomically duplicates `oldfd` onto `newfd`. If `newfd` is `1` (STDOUT), all subsequent `printf()` or `write(1, ...)` outputs are redirected to `oldfd` transparently.

---

## 3. Kernel Pipeline Mechanics (`pipeline_engine.c`)
- **Pipe Creation (`pipe(pipefd)`):** Allocates a 64 KB anonymous kernel RAM ring buffer. `pipefd[0]` is read-end, `pipefd[1]` is write-end.
- **Multi-Process Pipeline Routing:**
  - `Child 1` (`grep`): `dup2(fd_in, STDIN_FILENO)`, `dup2(pipefd[1], STDOUT_FILENO)`, `dup2(fd_err, STDERR_FILENO)`. Execs `grep ERROR`.
  - `Child 2` (`sort`): `dup2(pipefd[0], STDIN_FILENO)`, `dup2(fd_out, STDOUT_FILENO)`, `dup2(fd_err, STDERR_FILENO)`. Execs `sort -r`.
  - `Parent`: Closes all open FDs and waits for both child processes to exit via `wait(NULL)`.
