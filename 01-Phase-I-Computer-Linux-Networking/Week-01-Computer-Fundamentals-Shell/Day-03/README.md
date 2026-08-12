# Week 1 - Day 3: Threads, Concurrency & CPU Scheduling

---

## 📋 Objectives
- [x] Thread concept vs. Process
- [x] Shared address space (Shared Heap/Globals vs Private Stack per Thread)
- [x] Context switching overhead (Process vs Thread)
- [x] Mutex locks (`pthread_mutex_t`) & Race conditions
- [x] Build multithreaded chunk line counter capstone (`chunk_counter.c`)

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference & architectural overview of threads, locks, and SIMD. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX thread guides, and race condition mechanics. |
| 🛠️ [**`learning-materials/chunk_counter.c`**](learning-materials/chunk_counter.c) | Multithreaded C capstone reading 1 GB files in chunks with mutex synchronization. |
| 🧪 [**`learning-materials/race_condition.c`**](learning-materials/race_condition.c) | C program demonstrating unsynchronized shared memory data races. |

---

## ⚡ 1 GB Benchmark & How to Generate Test File Locally

To prevent committing large binary blobs to Git, `test_file.txt` is excluded via `.gitignore`. 

### Generate the 1 GB Test File Locally:
Run this command in your terminal inside `Day-03/learning-materials/`:

```bash
# Generate 1 GB (1,073,741,824 bytes) of text with newlines
python3 -c "
with open('test_file.txt', 'wb') as f:
    line = b'The quick brown fox jumps over the lazy dog\n'
    target = 1073741824
    written = 0
    while written < target:
        f.write(line)
        written += len(line)
"
```

### Benchmark Results (1 GB File / 8,390,835 Newlines):

```bash
# 1. Compile custom multithreaded C code with O3 optimization
gcc -O3 chunk_counter.c -o chunk_counter -lpthread

# 2. Run custom multithreaded chunk counter (4 threads)
time ./chunk_counter test_file.txt
# Output: Target File Size: 1073741824 bytes | Total Newlines: 8390835 | real: 4.976s

# 3. Run GNU wc -l
time wc -l test_file.txt
# Output: 8390835 test_file.txt | real: 1.235s
```

### 💡 Why GNU `wc -l` Beat Multithreaded C by ~4 Seconds:
1. **SIMD Vectorization (AVX2 / AVX-512):** GNU `wc` processes **32 to 64 bytes in a single CPU instruction** instead of looping byte-by-byte.
2. **Zero Mutex & Context-Switch Overhead:** GNU `wc` runs as a single-threaded stream processor, eliminating thread spawn, context switching, and lock contention.

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Thread Primitives & Multithreading Mechanics]]` in `Engineers-Playbook/02 Permanent/`
