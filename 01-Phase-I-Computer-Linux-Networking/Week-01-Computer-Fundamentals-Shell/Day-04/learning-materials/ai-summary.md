# Day 4 Architectural Overview & Technical Reference

> **Scope:** High-level architectural summary of Linux I/O streams (FD 0, 1, 2), file descriptor tables, system call redirection (`openat`, `dup2`), process piping (`pipe()`), exit statuses, and pipeline orchestration.

---

## 🌐 Shell Redirection & Pipeline System Call Flow

```text
[ Shell Terminal (Parent Process) ]
         │
         ├── 1. pipe(pipefd)  ──► Kernel creates ring buffer: pipefd[0] (read), pipefd[1] (write)
         │
         ├── 2. fork() Child 1 (Left Command: ls)
         │      ├── dup2(pipefd[1], STDOUT_FILENO)  ──► Redirects STDOUT (1) to Pipe Write-End
         │      └── execve("ls")                   ──► Replaces child memory with ls
         │
         ├── 3. fork() Child 2 (Right Command: grep)
         │      ├── dup2(pipefd[0], STDIN_FILENO)   ──► Redirects STDIN (0) to Pipe Read-End
         │      └── execve("grep")                 ──► Replaces child memory with grep
         │
         └── 4. waitpid(pid1) & waitpid(pid2)       ──► Reaps child processes after completion
```

---

## 1. File Descriptor Table & Standard Streams
- Every Linux process is initialized by POSIX convention with 3 default File Descriptors in its process table:
  - **`0` (STDIN):** Standard Input (default: Keyboard).
  - **`1` (STDOUT):** Standard Output (default: Terminal display).
  - **`2` (STDERR):** Standard Error (default: Terminal display, unbuffered for diagnostics).
- File descriptors `3`, `4`, `5`, ... are dynamically allocated by the kernel when opening files, sockets, or pipes.

---

## 2. Redirection Mechanics (`openat`, `dup2`)
- **Redirection Operator (`>`):** When executing `cat > file.txt`, the shell opens `file.txt` using `openat()` with flags `O_WRONLY|O_CREAT|O_TRUNC`, receiving FD `3`.
- **`dup2(3, 1)`:** Duplicates FD `3` onto FD `1` (`STDOUT`). Now, anything written to stdout is routed directly to `file.txt`.
- **`execve()`:** The shell executes `cat`. The `cat` binary simply writes to FD `1`, completely unaware that stdout has been redirected to a file on disk.

---

## 3. Pipeline Mechanics (`pipe()`)
- A pipe (`|`) connects the output of one process to the input of another via kernel memory.
- `pipe(pipefd)` creates an internal kernel ring buffer returning two file descriptors:
  - `pipefd[0]`: Read end of the pipe.
  - `pipefd[1]`: Write end of the pipe.
- Both child processes close unused pipe ends before `execve()` to ensure proper `EOF` (End-Of-File) signal delivery when the writer finishes.

---

## 4. Exit Status & Diagnostic Routing
- Every process returns an 8-bit exit status ($0$ to $255$).
- **Convention:** Exit status `0` indicates success; non-zero values ($1$ to $255$) indicate specific error conditions.
- Diagnostic logs and error messages must be routed to `STDERR` (`>&2`) so error output does not pollute data streams flowing down a pipe.
