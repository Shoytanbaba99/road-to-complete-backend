# Week 2 - Day 7 Review & Capstone Technical Reference

> **Scope:** Full architectural synthesis of Week 2 primitives (Stack vs. Heap Memory, Virtual Addresses, 4 KB Pages, Page Faults, File Descriptors, Ring 0 vs. Ring 3 Syscalls, Page Cache Buffering, Monotonic Clocks, and Process Debugging via `ptrace`).

---

## 🌐 Week 2 Master Architecture Diagram

```text
[ CPU INSTRUCTION (Ring 3 User Mode) ]
   ├── Stack Segment (Local variables & call frames)
   └── Heap Segment  (Dynamic malloc persistence)
            │
            ▼ (MMU Translation & 4 KB Page Tables)
[ PHYSICAL RAM & PAGE CACHE ] ◄── Page Fault Handler (Demand Paging)
            │
            ▼ (syscall instruction: Ring 3 ➔ Ring 0 Transition)
[ LINUX KERNEL (Ring 0) ]
   ├── Syscall Vector Table (sys_call_table[RAX])
   ├── Per-Process FD Table ➔ Open File Description ➔ VFS Inode
   └── Monotonic Hardware Clock (CLOCK_MONOTONIC for timeouts)
```

---

## 1. The Kernel Tracing Mechanism (`ptrace`)
- **`ptrace(PTRACE_TRACEME)`:** Child process invites parent process to trace its execution.
- **`ptrace(PTRACE_SYSCALL)`:** Pauses the child process right before and right after every system call trap.
- **`ptrace(PTRACE_GETREGS)`:** Reads physical CPU registers from the child's `user_regs_struct`:
  - `regs.orig_rax`: System call integer ID (e.g. `1` = `sys_write`, `59` = `sys_execve`).
  - `regs.rdi`: First argument passed to the system call (e.g. File Descriptor number).

---

## 2. Week 2 Deliverable Summary
- **1-Page OS Mental Model:** Rebuilt mental model of how the Linux Kernel mediates execution, virtual address spaces, page faults, standard stream descriptors, and system call traps.
- **`mini_strace.c` Capstone:** Built custom system call tracer capturing `sys_write`, `sys_execve`, and `sys_exit_group` traps in real-time.
