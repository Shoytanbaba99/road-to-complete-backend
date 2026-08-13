# Week 2 - Day 2: Virtual Memory Architecture, Page Tables & Page Faults

---

## 📋 Objectives
- [x] Virtual memory concept & virtual addresses vs physical addresses
- [x] Fixed-size 4 KB memory pages & Page Tables
- [x] MMU (Memory Management Unit) hardware translation
- [x] Page Faults & Demand Paging mechanics
- [ ] Custom user-space memory allocator capstone (`mini_malloc.c` with `sbrk`) *(Optional - Pending Review)*

---

## 🗺️ Day 2 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of Virtual Memory, Page Tables, Page Faults, and `sbrk()` allocators. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, MMU page translation guides, and heap allocation mechanics. |
| 🛠️ [**`learning-materials/mini_malloc.c`**](learning-materials/mini_malloc.c) | User-space memory allocator in C implementing `custom_malloc` / `custom_free` via `sbrk()`. |
| 🔬 [**`learning-materials/virtual_memory.c`**](learning-materials/virtual_memory.c) | C program demonstrating demand paging, page faults, and virtual address page boundaries. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile and run virtual memory demand paging experiment
gcc virtual_memory.c -o virtual_memory && ./virtual_memory

# Compile and run mini_malloc allocator test
gcc mini_malloc.c -o mini_malloc && ./mini_malloc
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Virtual Memory, Page Tables & Demand Paging]]` in `Engineers-Playbook/02 Permanent/`
