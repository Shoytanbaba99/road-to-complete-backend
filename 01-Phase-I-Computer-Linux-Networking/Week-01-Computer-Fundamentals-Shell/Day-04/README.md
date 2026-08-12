# Week 1 - Day 4: Shell Fundamentals, I/O Redirection & Pipelines

---

## 📋 Objectives
- [x] Shell fundamentals & token parsing
- [x] File descriptors (STDIN 0, STDOUT 1, STDERR 2)
- [x] Redirection mechanics (`>`, `<`, `>>`, `2>`) via `openat()` and `dup2()`
- [x] Process piping (`|`) via `pipe()`, `fork()`, and `dup2()`
- [x] Exit statuses ($0$ = Success, non-zero = Errors)
- [x] Diagnostic routing (`>&2`), `tee`, and `xargs`
- [x] Build C pipeline capstone (`pipeline.c`) implementing `ls | grep txt`

---

## 🗺️ Day 4 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of Linux file descriptors, syscalls, and pipelines. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX stream guides, and system call breakdowns. |
| 🛠️ [**`learning-materials/pipeline.c`**](learning-materials/pipeline.c) | C program implementing `ls | grep txt` using raw `pipe()`, `fork()`, `dup2()`, and `waitpid()`. |
| 📜 [**`learning-materials/processor.sh`**](learning-materials/processor.sh) | Bash script demonstrating argument validation, STDERR routing, and exit codes. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Trace shell system calls during redirection (openat, dup2, execve)
strace -e trace=openat,dup2,execve bash -c 'cat > file.txt'

# Run custom C pipeline capstone
gcc pipeline.c -o pipeline && ./pipeline

# Test bash diagnostic routing script with custom exit status
./processor.sh fail
echo "Exit Status: $?"
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[File Descriptors, Redirection & Kernel Pipelines]]` in `Engineers-Playbook/02 Permanent/`
