# Week 6 - Day 2 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Advanced Git Workflows: `git reflog` (Local Reference Log & Unreachable Commit Recovery) and `git bisect` ($O(\log N)$ Binary Search Regression Debugging).

---

## 🌐 `git reflog` & `git bisect` Workflow Reference

```text
[ GIT REFLOG: LOCAL HEAD MOVEMENT RECORD ]
  HEAD@{0}: reset: moving to HEAD~3      <-- Catastrophic reset!
  HEAD@{1}: commit: feat: add broken feature
  HEAD@{2}: commit: feat: stable baseline (SHA: 49d834f...)

  RECOVERY: git checkout -b recovery-branch 49d834f

[ GIT BISECT: O(log N) BINARY SEARCH ]
  Commit 100 (GOOD)  ──── Midpoint (Commit 50) ────► Commit 1 (BAD)
                           │ (Run Test: FAIL)
                           ▼
                     New Midpoint (Commit 75) ────► Commit 1 (BAD)
                           │ (Run Test: PASS)
                           ▼
                     Faulty Commit Pinpointed! (Commit 63)
```

---

## 1. Useful Workflows & Emergency Commands

| Command | Action | Use Case |
|---|---|---|
| `git reflog` | Displays timestamped history of all `HEAD` movements | Finding "lost" commits after `reset --hard` or deleted branches. |
| `git reset --hard HEAD@{n}` | Restores repo state to a specific reflog entry | Reverting accidental destructive operations. |
| `git bisect start <bad> <good>` | Initializes binary search for regression debugging | Finding which commit introduced a bug among hundreds of commits. |
| `git bisect run <script.sh>` | Automates binary search using test exit code (`0` = good, `1` = bad) | Hands-free automated bug isolation across large commit histories. |
| `git cherry-pick <commit>` | Applies specific commit diff onto current branch | Pulling a single bug fix without merging an entire branch. |
