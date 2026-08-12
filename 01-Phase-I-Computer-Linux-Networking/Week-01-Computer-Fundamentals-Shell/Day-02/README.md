# Week 1 - Day 2: Programs vs. Processes & Process Anatomy

---

## 📋 Objectives
- [x] Program vs. Process
- [x] Executable vs. Source code (ELF format)
- [x] Process address space (`/proc/[pid]/maps`)
- [x] PID / PPID process tree hierarchy (`pstree`)
- [x] Inspect process states (`R`, `S`, `Z` Zombie) with `ps` and `top`/`htop`
- [x] Build custom `/proc` process inspector (`ps.py`)

---

## 🗺️ Day 2 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference & architectural summary of all Day 2 concepts. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, Linux command guides, and process breakdown. |
| 🛠️ [**`learning-materials/ps.py`**](learning-materials/ps.py) | Custom Python process inspector querying `/proc/[pid]/status` directly. |
| 🔬 [**`learning-materials/process_anatomy.c`**](learning-materials/process_anatomy.c) | C program demonstrating memory address space layout and zombie process creation. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Inspect ELF binary headers and entry point
readelf -h /bin/ls

# View system process tree hierarchy
pstree

# Inspect virtual memory address space of a living process
cat /proc/[pid]/maps

# Inspect zombie process state
ps -stat Z
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Process Architecture & Memory Address Space]]` in `Engineers-Playbook/02 Permanent/`
