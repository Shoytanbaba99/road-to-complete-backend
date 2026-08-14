# Week 2 - Day 3: File Descriptors, Standard Streams & Kernel Pipelines

---

## 📋 Objectives
- [x] File Descriptor table architecture (Per-process table ➔ Open file descriptions ➔ Inodes)
- [x] Standard descriptors (`0 STDIN`, `1 STDOUT`, `2 STDERR`)
- [x] Stream redirection mechanics (`dup2`)
- [x] Files, sockets, and pipes as unified OS resources
- [x] Build C multi-stage pipeline engine ([`pipeline_engine.c`](learning-materials/pipeline_engine.c))

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of FD lookup hierarchy, stream redirection, and pipe IPC. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX I/O syscall specifications, and pipe mechanics. |
| 🛠️ [**`learning-materials/fd_manipulator.c`**](learning-materials/fd_manipulator.c) | C program demonstrating `dup2()` stdout hijacking and invalid FD error handling. |
| ⚙️ [**`learning-materials/pipeline_engine.c`**](learning-materials/pipeline_engine.c) | Multi-child process pipeline in C executing `grep ERROR input.txt \| sort -r > output.txt 2> error.txt`. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile and run FD stdout manipulator
gcc fd_manipulator.c -o fd_manipulator && ./fd_manipulator

# Compile and run multi-stage pipeline engine
gcc pipeline_engine.c -o pipeline_engine && ./pipeline_engine
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[File Descriptors, Standard Streams & Pipeline Routing]]` in `Engineers-Playbook/02 Permanent/`
