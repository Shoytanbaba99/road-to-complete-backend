# Week 2 - Day 1: Stack vs. Heap Memory & Dynamic Allocation

---

## 📋 Objectives
- [x] Stack segment concept (Auto-allocation, function call frames, downward growth)
- [x] Heap segment concept (Dynamic allocation, manual lifecycle, upward growth)
- [x] Memory functions (`malloc`, `calloc`, `realloc`, `free`)
- [x] Pointer dereferencing & address inspection
- [x] Build C dynamic linked list capstone ([`dynamic_linked_list.c`](learning-materials/dynamic_linked_list.c))

---

## 🗺️ Day 1 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of Stack vs. Heap architecture, pointers, and memory allocators. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX memory allocation guides, and pointer mechanics. |
| 🛠️ [**`learning-materials/dynamic_linked_list.c`**](learning-materials/dynamic_linked_list.c) | C program building a dynamic linked list using `malloc()` and freeing nodes safely. |
| 🔬 [**`learning-materials/memory_anatomy.c`**](learning-materials/memory_anatomy.c) | C program printing raw pointer memory addresses to prove stack downward vs heap upward growth. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile and run dynamic linked list capstone
gcc dynamic_linked_list.c -o dynamic_linked_list && ./dynamic_linked_list

# Compile and run memory anatomy program (observing Stack vs Heap addresses)
gcc memory_anatomy.c -o memory_anatomy && ./memory_anatomy
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Stack vs Heap Memory & Dynamic Allocation]]` in `Engineers-Playbook/02 Permanent/`
