# Week 2 - Day 6: Wall-Clock vs. Monotonic Clocks & Resilient Timeouts

---

## 📋 Objectives
- [x] Wall-Clock time (`CLOCK_REALTIME`) vs Monotonic time (`CLOCK_MONOTONIC`)
- [x] Why wall-clock time is dangerous for measuring durations & SLAs (NTP jumps)
- [x] High-precision timing with `clock_gettime()` & `nanosleep()`
- [x] Build C resilient process timeout wrapper ([`resilient_wrapper.c`](learning-materials/resilient_wrapper.c))

---

## 🗺️ Day 6 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of `CLOCK_MONOTONIC` vs. `CLOCK_REALTIME`, NTP skew, and timeouts. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX timing syscall specifications, and clock resolution rules. |
| 🛠️ [**`learning-materials/resilient_wrapper.c`**](learning-materials/resilient_wrapper.c) | **Capstone:** C process wrapper executing child commands with strict `CLOCK_MONOTONIC` timeout enforcing `SIGKILL`. |
| 🔬 [**`learning-materials/clock_profiler.c`**](learning-materials/clock_profiler.c) | Dual-clock profiling demonstrator comparing REALTIME vs MONOTONIC across sleep intervals. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile and test resilient process timeout wrapper
gcc resilient_wrapper.c -o resilient_wrapper

# Run command with 2-second timeout (will complete successfully)
./resilient_wrapper 2.0 sleep 1

# Run command with 2-second timeout (will exceed timeout and be killed by SIGKILL)
./resilient_wrapper 2.0 sleep 5

# Inspect clock profiler
gcc clock_profiler.c -o clock_profiler && ./clock_profiler
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Wall Clock vs Monotonic Clocks & Timeout Mechanics]]` in `Engineers-Playbook/02 Permanent/`
