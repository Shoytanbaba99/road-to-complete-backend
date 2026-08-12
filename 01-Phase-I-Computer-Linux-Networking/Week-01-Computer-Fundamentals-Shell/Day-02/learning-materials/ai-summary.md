# Day 2 Architectural Overview & Technical Reference

> **Scope:** High-level architectural summary of Day 2 operating system process primitives, ELF binary structures, virtual address space segmentation, process hierarchies, and kernel lifecycle management.

---

## 🌐 Process Anatomy & Execution Pipeline

```text
[ Static Source Code (.c / .py / .rs) ]
                 │
                 ▼ (Compilation / Translation)
[ Executable Binary (ELF64) ] ──(readelf Entry Point)──► CPU Program Counter
                 │
                 ▼ (OS Execution / fork() + exec())
[ Living Process ] (Assigned PID & PPID by OS Kernel)
                 │
                 ├── Virtual Address Space (/proc/[pid]/maps)
                 │     ├── Text Segment (r-xp: Machine Code)
                 │     ├── Heap Segment (rw-p: Dynamic malloc() Memory)
                 │     └── Stack Segment (rw-p: Local Variables & Call Frames)
                 │
                 └── Kernel Lifecycle (Active ➔ Exit ➔ Zombie (Z) ➔ Reaped via wait())
```

---

## 1. Source Code vs. Executable Binaries (ELF Format)
- **Source Code:** Human-readable text files containing high-level logic. Dead on disk until translated.
- **Executable (ELF64):** Executable and Linkable Format, the standard binary format for Linux executables and shared libraries.
- **Entry Point Address (`readelf -h /bin/ls`):** The ELF header explicitly defines the `Entry point address` (e.g., `0x3910`). Upon program execution, the OS loader maps the ELF binary into memory and sets the CPU's Program Counter (PC) to this exact virtual address to begin execution.

---

## 2. Program vs. Process
- **Program:** A passive, static file resting on secondary storage (SSD/HDD), consuming zero CPU cycles and holding no active system state.
- **Process:** A living, active entity executing machine instructions in RAM. Assigned a unique **Process ID (PID)** and **Parent Process ID (PPID)**, possessing an isolated virtual address space, file descriptor table, registers, and environment variables.

---

## 3. Process Address Space Segmentation (`/proc/[pid]/maps`)
To guarantee process isolation, the operating system uses virtual memory (managed by the Memory Management Unit - MMU) to map fake virtual addresses to physical RAM:

| Memory Segment | Permissions | Purpose & Characteristics |
|---|---|---|
| **Text Segment** | `r-xp` (Read / Execute) | Contains compiled, read-only machine code instructions loaded directly from the ELF binary. |
| **Data Segment** | `rw-p` (Read / Write) | Contains explicitly initialized global and static variables. |
| **BSS Segment** | `rw-p` (Read / Write) | Contains uninitialized global and static variables, automatically zeroed out on boot. |
| **Heap** | `rw-p` (Read / Write) | Dynamically allocated runtime memory (`malloc()` / `free()`). Grows upward in memory. |
| **Stack** | `rw-p` (Read / Write) | Stores function parameters, local variables, and return pointers. Grows downward in memory. |

---

## 4. Process Hierarchy & Lifecycle Management

### A. Process Creation (`fork()` & `exec()`)
- In Linux, new processes are created when an existing parent process calls `fork()`.
- `fork()` clones the parent process. It returns `0` to the child process and the child's PID to the parent process.
- All processes trace their ancestral hierarchy back to **PID 1 (`systemd` or `init`)**, visible using `pstree`.

### B. Zombie Processes (`Z` / `<defunct>` State)
- When a child process terminates via `exit()`, it enters the **Zombie (`Z`)** state.
- The Linux kernel retains a small entry in the process table holding the child's exit status.
- The parent process MUST call `wait()` or `waitpid()` to read the exit status and allow the kernel to reap the zombie.
- If a parent sleeps or ignores the dead child without calling `wait()`, the process remains a Zombie, leaking PID entries in the OS kernel table.

---

## 5. Kernel Inspection via Pseudo-Filesystem (`/proc`)
- The `/proc` directory is a virtual pseudo-filesystem created dynamically by the Linux kernel in RAM.
- `/proc/[pid]/status`: Exposes human-readable process status metadata (Name, State `R/S/Z`, PID, PPID, VmPeak memory).
- `/proc/[pid]/cmdline`: Contains the exact raw command and arguments used to launch the process.
- **Custom Inspector (`ps.py`):** Acts as a user-space wrapper by directly parsing raw text from `/proc/[pid]/status` and `/proc/[pid]/cmdline` without invoking external binaries.
