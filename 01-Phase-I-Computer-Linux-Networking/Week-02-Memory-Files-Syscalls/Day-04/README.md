# Week 2 - Day 4: System Calls, Privilege Rings & `strace` Tracing

---

## 📋 Objectives
- [x] User Mode (Ring 3) vs. Kernel Mode (Ring 0) CPU privilege levels
- [x] System Call entry mechanics (`syscall` instruction & `sys_call_table`)
- [x] Tracing process system calls with `strace`
- [x] Raw syscall invocation bypassing `glibc` wrappers ([`raw_syscall.c`](learning-materials/raw_syscall.c))
- [x] Build C file copier utility using raw I/O syscalls ([`my_cp.c`](learning-materials/my_cp.c))

---

## 🗺️ Day 4 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of CPU privilege rings, syscall vectors, and `strace`. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX syscall specifications, and kernel trap mechanics. |
| 🛠️ [**`learning-materials/raw_syscall.c`**](learning-materials/raw_syscall.c) | C program demonstrating `syscall(SYS_write, ...)` direct kernel traps and invalid pointer defenses. |
| ⚙️ [**`learning-materials/my_cp.c`**](learning-materials/my_cp.c) | C file copy utility using `open()`, `read()`, `write()`, and `close()` with partial-write loops. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile and run raw syscall demonstrator
gcc raw_syscall.c -o raw_syscall && ./raw_syscall

# Trace system calls of my_cp file copier
gcc my_cp.c -o my_cp
strace -e openat,read,write,close ./my_cp source.txt dest.txt
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[System Calls, User vs Kernel Mode & strace Tracing]]` in `Engineers-Playbook/02 Permanent/`
