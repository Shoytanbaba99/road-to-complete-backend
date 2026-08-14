# Week 2 - Day 4 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of CPU Privilege Rings (Ring 0 Kernel Mode vs. Ring 3 User Mode), System Call Entry Mechanics (`syscall` assembly instruction), System Call Vector Tables, `strace` Inspection, and Low-Level I/O Syscalls (`open`, `read`, `write`, `close`).

---

## 🌐 User Mode (Ring 3) vs. Kernel Mode (Ring 0) Architecture

```text
[ USER SPACE (Ring 3) ]
  ├── C Application (printf / my_cp)
  └── glibc Wrapper Functions (open, read, write)
          │
          ▼ (syscall assembly instruction: RAX = Syscall Number)
────────────────────────────────────────────────────────────────
[ KERNEL SPACE (Ring 0) ]
  ├── System Call Vector Table (sys_call_table[RAX])
  ├── Security & Permission Check (VFS Inode / Memory Boundary)
  └── Hardware Execution (SSD Write / NIC Packet Out)
```

---

## 1. CPU Privilege Rings
- **Ring 3 (User Mode):** Restricted execution level where applications run. Applications cannot touch physical hardware registers, execute privileged CPU instructions, or access raw memory directly.
- **Ring 0 (Kernel Mode):** Unrestricted privilege level where the OS kernel executes. Has full control over physical hardware, RAM page tables, and CPU registers.

---

## 2. System Call Execution Mechanics
1. **Syscall ID Loading:** The application loads the target system call integer ID (e.g. `SYS_write` = 1 on x86_64) into CPU register `RAX`.
2. **Mode Switch Instruction:** The CPU executes `syscall` (or `sysenter` / `int 0x80`), causing a trap hardware interrupt. The CPU instantly switches from Ring 3 to Ring 0.
3. **Syscall Table Lookup:** The kernel looks up `sys_call_table[RAX]` in kernel space and executes the associated C function (`sys_write()`).
4. **Return to User Mode:** The kernel stores the result in register `RAX` and executes `sysret` to return the CPU back to Ring 3 User Mode.

---

## 3. Tracing System Calls (`strace`)
- `strace` uses the `ptrace()` system call to attach to a process, intercepting every Ring 3 $\rightarrow$ Ring 0 transition and printing the syscall name, arguments, and return values in real-time.

---

## 4. Raw System Call Invocation (`raw_syscall.c` & `my_cp.c`)
- **`syscall(SYS_write, ...)`:** Bypasses C standard library wrappers to issue direct system call traps to the Linux kernel.
- **`my_cp.c`:** Demonstrates efficient low-level file copying using 1 KB RAM buffers and loop-guarded `write()` calls to handle partial kernel writes safely.
