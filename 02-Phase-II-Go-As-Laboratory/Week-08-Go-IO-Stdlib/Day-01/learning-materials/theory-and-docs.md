### Phase 1: The Generation Trap

**The Core Problem:**
A program needs to persist state to non-volatile storage (disk) or stream data across network file descriptors.

In systems programming, transferring bytes between User Mode (Ring 3) and the Kernel/Storage (Ring 0) relies on low-level system calls (`read`, `write`). Every individual system call forces a hardware context switch: saving CPU registers, swapping page tables, changing privilege levels, executing kernel interrupt handlers, and returning back to user space.

Imagine a program processing a 100 MB file.

**Your Task:**
If you were the systems engineer tasked with designing a file-reading and writing subsystem from scratch:

1. What naive approach would you take to read or write a large data file character-by-character or chunk-by-chunk?
2. Where and why would that naive approach degrade performance or exhaust OS resources under heavy system load?

_I am waiting for your answer._
