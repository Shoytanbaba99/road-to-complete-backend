# Week 2 - Day 6 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Monotonic vs. Wall-Clock Time, `CLOCK_MONOTONIC` vs. `CLOCK_REALTIME`, NTP Time Slew / Leap Second adjustments, Timeouts, and Non-blocking Process Timeout Wrappers (`resilient_wrapper.c`).

---

## 🌐 Wall-Clock (`CLOCK_REALTIME`) vs. Monotonic Clock (`CLOCK_MONOTONIC`)

```text
[ WALL-CLOCK TIME (CLOCK_REALTIME) ]
  ├── Calendar / Human Time (e.g. 2026-08-15 14:45:00 UTC)
  └── Subject to NTP Network Sync, Manual Overrides, DST, and Backward Jumps!
      └── DANGEROUS for calculating time intervals (End - Start can be NEGATIVE!)

[ MONOTONIC CLOCK (CLOCK_MONOTONIC) ]
  ├── Nanoseconds since Kernel Boot (Tick Count)
  └── Strictly Monotonic (Always moves forward at a constant hardware frequency)
      └── SAFE & MANDATORY for timeouts, rate limiters, performance profiling, and SLAs!
```

---

## 1. Why Wall-Clock Time is Dangerous for Durations
- **NTP Time Slew & Clock Skew:** When network time protocol (NTP) daemons synchronize system clocks, `CLOCK_REALTIME` can jump backward or forward in time.
- **Negative Durations:** If `end_time` is captured after an NTP backward time jump, `end_time - start_time` yields a negative number, breaking database connection pools, lock timeouts, and rate limiters.

---

## 2. Monotonic Clocks (`CLOCK_MONOTONIC`)
- **System Boot Counter:** Measures hardware clock ticks since system boot.
- **Guaranteed Invariant:** `CLOCK_MONOTONIC` never jumps backward regardless of system time changes.

---

## 3. Resilient Process Timeout Wrapper (`resilient_wrapper.c`)
- **`fork()` & `execvp()`:** Spawns a target command in a child process.
- **`clock_gettime(CLOCK_MONOTONIC, &ts)`:** Captures initial nanosecond timestamp.
- **Non-blocking Loop:** Polls `waitpid(child_pid, &status, WNOHANG)`.
- **Enforcing Timeout:** Calculates `elapsed_time` via `CLOCK_MONOTONIC`. If `elapsed_time >= timeout`, sends `kill(child_pid, SIGKILL)` to terminate long-running or frozen processes cleanly.
