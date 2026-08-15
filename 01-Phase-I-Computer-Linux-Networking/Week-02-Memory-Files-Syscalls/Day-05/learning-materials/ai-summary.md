# Week 2 - Day 5 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of File I/O Buffering, `glibc` User-Space Buffers vs. Kernel Page Cache, Dirty Page Flushing, `fsync()` Syscall, Inode Metadata Inspection (`fstat`), and Safe Secure File Deletion (`shred_lite.c`).

---

## 🌐 Three-Tier I/O Buffering & Persistence Pipeline

```text
[ USER SPACE (Application) ]
  └── stdio Buffer (glibc fwrite / fprintf - 8 KB RAM buffer)
          │ (Flushed on buffer full / \n / fflush)
          ▼
[ KERNEL SPACE (Operating System) ]
  └── Linux Page Cache (Dirty Pages in System RAM)
          │ (Flushed by pdflush/writeback daemon or explicit fsync())
          ▼
[ PHYSICAL STORAGE (SSD / HDD) ]
  └── Disk Hardware Controller & Non-Volatile Memory Platters
```

---

## 1. User-Space vs. Kernel-Space Buffering
- **User-Space Buffering (`stdio`):** `glibc` functions like `fprintf()` write to an 8 KB user-space buffer in process RAM. Data is NOT sent to the kernel until the buffer fills or `fflush()` is called. If the process crashes before flushing, data is lost.
- **Kernel Page Cache:** Calling `write()` transfers bytes to kernel RAM dirty pages. If the process dies, the kernel still writes dirty pages to disk later. But if system power fails, dirty pages in RAM are lost.
- **Hardware Storage:** Physical flash cells / disk platters where non-volatile data resides.

---

## 2. Force Disk Flushing (`fsync`)
- `fsync(fd)` blocks the calling process until all dirty pages associated with file descriptor `fd` are physically written to the underlying storage device controller.

---

## 3. Secure File Erasure Capstone (`shred_lite.c`)
- **`fstat(fd, &st)`:** Inspects file metadata, inode number (`st.st_ino`), regular file check (`S_ISREG`), and total size (`st.st_size`).
- **Pattern Overwriting:** Overwrites file blocks in 64 KB chunks (`CHUNK_SIZE 65536`) with `0xFF` bytes in a loop.
- **Physical Sync & Unlink:** Invokes `fsync(fd)` to guarantee physical disk platter overwrite before calling `unlink(path)` to delete the dentry.
