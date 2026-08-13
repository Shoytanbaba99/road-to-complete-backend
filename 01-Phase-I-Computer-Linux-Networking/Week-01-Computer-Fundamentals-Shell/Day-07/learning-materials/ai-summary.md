# Week 1 - Day 7 Review & Capstone Technical Reference

> **Scope:** Architectural synthesis of Week 1 Unix primitives (CPU/RAM, Programs vs. Processes, Threads vs. Processes, File Descriptors, Pipelines, Filesystem Inodes, and Environment Inheritance) and Daemon Process Detachment.

---

## 🌐 Week 1 Master Architecture Diagram

```text
[ HARDWARE (Day 1) ]  CPU Cache Lines (64B) ➔ RAM ➔ MMIO Devices
        │
        ▼
[ OS KERNEL & PROCESSES (Day 2) ]  ELF64 Binaries ➔ Virtual Address Space (4KB Pages) ➔ PID/PPID
        │
        ▼
[ THREADS & CONCURRENCY (Day 3) ]  Shared Heap/Code + Private Stacks ➔ Mutex Locks ➔ SIMD
        │
        ▼
[ I/O & PIPELINES (Day 4) ]  FD 0, 1, 2 ➔ dup2() Redirection ➔ pipe() Ring Buffers
        │
        ▼
[ FILESYSTEM & INODES (Day 5) ]  FHS Tree ➔ Inodes (Metadata/Blocks) ➔ Dentries (Filename Maps)
        │
        ▼
[ ENVIRONMENT & SCRIPTS (Day 6) ]  execve(path, argv, envp) ➔ Top-down envp ➔ $PATH Resolution
        │
        ▼
[ DAEMONIZATION & TOOLING (Day 7) ]  env -i ➔ /dev/null STDIN ➔ File STDOUT/STDERR ➔ PID tracking ($!)
```

---

## 1. What is a Daemon Process?
A **Daemon** is a long-running background process that operates independently of any active user terminal session (e.g. `systemd`, `postgres`, `nginx`, `dockerd`).

### Key Requirements of a Daemon:
1. **Detached Terminal I/O:** Redirects `STDIN` from `/dev/null` to prevent blocking on keyboard input. Redirects `STDOUT` and `STDERR` to log files on disk.
2. **Environment Sanitization (`env -i`):** Clears untrusted environment variables and explicitly sets a safe `$PATH` (`/usr/bin:/bin`).
3. **Background Spawning (`&`):** Executes in the background, freeing the controlling terminal.
4. **PID File Tracking (`/tmp/daemon.pid`):** Writes `$!` (the background Process ID) to a PID file so process managers can monitor or send termination signals (`SIGTERM`).

---

## 2. Shell Script Production Patterns (`set -euo pipefail`)
Production bash scripts use `set -euo pipefail` for strict error handling:
- **`set -e`:** Exit immediately if any command returns a non-zero exit status.
- **`set -u`:** Treat unset environment variables as an error and exit immediately.
- **`set -o pipefail`:** Return the exit status of the first failed command in a pipe, preventing pipeline errors from being swallowed.
