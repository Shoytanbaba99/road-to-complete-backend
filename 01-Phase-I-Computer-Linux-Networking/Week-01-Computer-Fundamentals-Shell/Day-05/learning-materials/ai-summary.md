# Day 5 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Linux Filesystem Hierarchy (FHS), Inodes vs. Dentries, File Permission Octals, Ownership (`chmod`/`chown`), Hard vs. Soft (Symbolic) Links, Sticky Bits, and the `stat()` system call.

---

## 🌐 Inode vs. Dentry File System Architecture

```text
[ DIRECTORY ENTRY (Dentry) ]
├── Filename: "original.txt"  ──► Points to Inode #123456
└── Filename: "hardlink.txt"  ──► Points to Inode #123456 (st_nlink = 2)

                              │
                              ▼
                  [ INODE #123456 METADATA ]
                  ├── File Type & Mode (Permissions: rwxr-xr-x)
                  ├── Owner UID & Group GID
                  ├── File Size (Bytes)
                  ├── Link Count (st_nlink)
                  └── Block Pointers (Pointers to physical SSD/HDD blocks)
```

---

## 1. Filesystem Hierarchy Standard (FHS)
- **`/` (Root):** Root of the single-tree virtual file system.
- **`/bin` & `/sbin`:** Essential system binaries (user vs admin commands).
- **`/etc`:** System-wide configuration files.
- **`/var`:** Variable data files (logs, databases, spool files).
- **`/dev`:** Device files acting as interfaces to physical hardware.
- **`/proc` & `/sys`:** Pseudo-filesystems exposing live kernel state in RAM.

---

## 2. Inodes vs. Dentries (Directory Entries)
- **Inode:** The kernel data structure storing all file metadata (size, permissions, UID/GID, block addresses). **Inodes store NO filenames.**
- **Dentry:** Maps human-readable filenames to Inode numbers inside directory structures.

---

## 3. Hard Links vs. Soft (Symbolic) Links

| Feature | Hard Link (`ln target link`) | Soft / Symbolic Link (`ln -s target link`) |
|---|---|---|
| **Structure** | A new Dentry pointing to the **same Inode number**. | A new Inode storing the target file path string. |
| **Inode Number** | Identical to target Inode. | Unique new Inode number. |
| **Link Count (`st_nlink`)** | Increments by 1. | Does not increment target's link count. |
| **Target Deletion** | File data persists until link count drops to 0. | Link breaks ("dangling symlink"). |
| **Cross-Filesystem** | Cannot span different filesystems. | Can span across different filesystems/mounts. |

---

## 4. Permissions & Octal Modes (`chmod`)
- **3 Access Groups:** Owner (`u`), Group (`g`), Others (`o`).
- **Bit Weights:** Read (`r` = 4), Write (`w` = 2), Execute (`x` = 1).
- **Directory Execution (`x`):** Permission to enter/traverse the directory (`cd`).
- **Directory Write (`w`):** Permission to add, rename, or unlink dentries within the directory.
- **Sticky Bit (`1777` / `/tmp`):** Prevents non-owners from unlinking/deleting files inside a shared writeable directory.

---

## 5. Kernel Inspection via `stat()` System Call
- `stat(const char *pathname, struct stat *statbuf)` queries the filesystem for an inode's metadata:
  - `st_ino`: Inode ID number.
  - `st_mode`: Bitmask containing file type and permissions.
  - `st_nlink`: Number of hard links pointing to this inode.
  - `st_uid` / `st_gid`: Owner User ID and Group ID.
  - `st_size`: Total size in bytes.
