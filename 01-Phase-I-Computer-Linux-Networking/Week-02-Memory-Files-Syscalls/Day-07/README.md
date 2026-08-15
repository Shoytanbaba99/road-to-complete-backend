# Week 2 - Day 7: Review & Capstone — Process Debugging & System Call Interception

---

## 📋 Objectives
- [x] Rebuild complete Week 2 mental model without notes
- [x] Trace full program startup & file execution mechanics
- [x] Process debugging via `ptrace()` kernel interface
- [x] Intercept CPU system call traps and inspect hardware registers (`orig_rax`, `rdi`)
- [x] Build C system call interceptor capstone ([`mini_strace.c`](learning-materials/mini_strace.c))

---

## 🗺️ Day 7 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal 1-page OS mental model and Week 2 synthesis written from memory. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of Week 2 architecture and `ptrace()` register interception. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX debugging specifications, and `ptrace` register layouts. |
| 🛠️ [**`learning-materials/mini_strace.c`**](learning-materials/mini_strace.c) | **Capstone:** C system call tracer intercepting child process syscall traps and printing register IDs. |
| 🔬 [**`learning-materials/investigation_report.c`**](learning-materials/investigation_report.c) | System call investigation report program tracing file open, read, write, and exit sequences. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile and run mini_strace system call interceptor
gcc mini_strace.c -o mini_strace && ./mini_strace

# Compile investigation report program
gcc investigation_report.c -o investigation_report && ./investigation_report
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Process Interception, ptrace & System Call Debugging]]` in `Engineers-Playbook/02 Permanent/`
