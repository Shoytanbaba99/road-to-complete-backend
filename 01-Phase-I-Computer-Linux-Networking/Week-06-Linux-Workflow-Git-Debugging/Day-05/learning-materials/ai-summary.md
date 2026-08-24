# Week 6 - Day 5 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of MIT Missing Semester Tooling & End-to-End Debugging Workflows: Standard Streams (`stdout` FD 1 vs `stderr` FD 2), System Call Tracing (`strace`), Data Wrangling Pipelines (`grep`, `sed`, `awk`, `sort`, `uniq`), and Log Analysis ([`log_analyzer.sh`](missing-semester-lab/log_analyzer.sh)).

---

## 🌐 Data Wrangling & System Tracing Architecture

```text
[ UNKNOWN APP CRASH DIAGNOSIS ]
  $ strace -e trace=file ./mystery_app 2> strace_out.log
  Result: Pinpoints exact missing file path (openat ENOENT "config.json")

[ LOG DATA WRANGLING PIPELINE ]
  access.log ──► grep -oE 'PATH=[^ ]+' ──► cut -d'=' -f2 ──► sort ──► uniq -c ──► sort -rn ──► head -3
```

---

## 1. Missing Semester Linux Tooling Summary

| Tool / Stream | Purpose & Command Syntax | Use Case |
|---|---|---|
| **`stderr` (FD 2)** | Separate output stream for diagnostic & error logs | Prevents error messages from polluting piped data streams (`cmd > stdout.txt 2> stderr.log`). |
| **`strace`** | Intercepts & logs kernel system calls | Diagnosing why a binary fails at startup (missing files, failed socket binds). |
| **`grep` & `awk`** | Pattern matching & columnar text processing | Extracting key fields (IP addresses, HTTP status codes, latency numbers). |
| **`sort` & `uniq -c`** | Frequency counting & sorting pipeline | Calculating top URIs, most frequent IP hits, or error distribution. |
