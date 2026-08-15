# Week 2 - Day 5: File I/O Buffering, Page Cache & Filesystem Metadata

---

## 📋 Objectives
- [x] Basic I/O syscalls (`open`, `read`, `write`, `close`)
- [x] Three-tier I/O buffering architecture (User-Space stdio vs. Kernel Page Cache vs. Disk Controller)
- [x] Data persistence & dirty page flushing mechanics (`fsync`)
- [x] Inode metadata inspection (`fstat`, `st_size`, `st_ino`, `S_ISREG`)
- [x] Build C secure file eraser capstone ([`shred_lite.c`](learning-materials/shred_lite.c))

---

## 🗺️ Day 5 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of stdio vs. Page Cache, `fsync()`, and inode metadata. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX file I/O specifications, and page cache flush mechanics. |
| 🛠️ [**`learning-materials/shred_lite.c`**](learning-materials/shred_lite.c) | **Capstone:** Secure file shredder overwriting file blocks with `0xFF` pattern, calling `fsync()`, and unlinking. |
| 🔬 [**`learning-materials/io_anatomy.c`**](learning-materials/io_anatomy.c) | C program demonstrating stdio user-space buffering vs raw unbuffered syscall performance. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile and run secure file eraser capstone
gcc shred_lite.c -o shred_lite
echo "Sensitive Secret Payload" > secret.txt
./shred_lite secret.txt

# Inspect I/O buffering anatomy
gcc io_anatomy.c -o io_anatomy && ./io_anatomy
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[File I/O Buffering, Page Cache & Metadata Persistence]]` in `Engineers-Playbook/02 Permanent/`
