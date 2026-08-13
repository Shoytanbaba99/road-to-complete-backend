# Week 1 - Day 5: Filesystem Hierarchy, Inodes & Link Mechanics

---

## 📋 Objectives
- [x] Filesystem Hierarchy Standard (FHS: `/`, `/etc`, `/var`, `/dev`, `/proc`, `/sys`)
- [x] Inode data structure vs. Dentry mapping
- [x] File permissions & Octal representation (`chmod 755`, `chown`)
- [x] Directory permission security mechanics (`w+x` dentry unlinking & Sticky Bit `1777`)
- [x] Hard links vs. Symbolic (Soft) links (`ln` vs `ln -s`)
- [x] Build C inode inspector capstone (`mini_stat.c`) querying `stat()`

---

## 🗺️ Day 5 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of Linux filesystems, inodes, and permission bitmasks. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, FHS guides, and permission security rules. |
| 🛠️ [**`learning-materials/mini_stat.c`**](learning-materials/mini_stat.c) | C program inspecting file metadata, inode numbers, link counts, and permission bitmasks via `stat()`. |
| 🔗 [**`learning-materials/hardlink.txt`**](learning-materials/hardlink.txt) | Hard link artifact sharing the exact inode number with `original.txt`. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Inspect file inode number and metadata
ls -li original.txt hardlink.txt

# Compile and run custom C inode inspector
gcc mini_stat.c -o mini_stat && ./mini_stat original.txt

# Create hard link vs soft link
ln original.txt hardlink.txt
ln -s original.txt softlink.txt

# Check sticky bit permission on /tmp
ls -ld /tmp
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Filesystem Hierarchy, Inodes & Permission Mechanics]]` in `Engineers-Playbook/02 Permanent/`
