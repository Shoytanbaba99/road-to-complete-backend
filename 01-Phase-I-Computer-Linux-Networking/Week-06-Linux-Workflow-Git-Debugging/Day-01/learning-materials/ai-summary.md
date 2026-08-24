# Week 6 - Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Git Internals: Git Object Database (`.git/objects/`), Object Types (Blob, Tree, Commit, Ref), `zlib` Compression, SHA-1 Hashing, 3-Way Merging, and Rebase Graph Rewriting.

---

## 🌐 Git DAG (Directed Acyclic Graph) Architecture

```text
[ GIT OBJECTS IN .git/objects/ ]

  HEAD (Ref: refs/heads/main)
    │
    ▼
  Commit Object (hash: 39d7200...)
  ├── Root Tree: 7a8b...
  ├── Parent Commit: b1d4...
  ├── Author: Monotheist <user@example.com>
  └── Message: "feat: add git internals"
        │
        ▼
  Tree Object (Directory Snapshot)
  ├── 100644 blob e69de2... README.md
  └── 040000 tree f91a00... src/
                │
                ▼
          Sub-Tree Object
          └── 100644 blob a7b21... main.go (zlib compressed content)
```

---

## 1. Object Types & Merge Mechanics

| Object / Operation | Underlying Storage / Mechanism | Purpose |
|---|---|---|
| **Blob** | `zlib` compressed raw bytes keyed by SHA-1 hash | Stores file content (no filenames, no metadata). |
| **Tree** | Array of `[mode, type, SHA-1, filename]` entries | Directory structure linking filenames to Blobs & Sub-Trees. |
| **Commit** | Text object containing Root Tree SHA-1, Parent SHA-1(s), Author & Timestamp | Project snapshot history & DAG node. |
| **Ref** | Text file inside `.git/refs/` containing 40-char SHA-1 string | Named pointer to a commit (Branch or Tag). |
| **3-Way Merge** | Finds common ancestor (Merge Base) + `HEAD` + Target tip | Combines changes; creates merge commit with **2 parent pointers**. |
| **Rebase** | Replays commit diffs sequentially on top of target branch | Linearizes history; produces **new commit SHA-1 hashes**. |
