Welcome to **Week 7, Day 2: Go Syntax Foundations — Variables/Constants, Functions, Control Flow, and `defer**`.

We are going to master how Go structures data storage in memory, controls execution paths, and guarantees resource cleanup.

---

### Phase 1: The Generation Trap

**The Core Problem:**
In systems programming (like C), functions often acquire resources—allocating dynamic memory on the heap, opening file descriptors (`open`), locking mutexes (`pthread_mutex_lock`), or opening network sockets (`socket`).

When a function has complex control flow with multiple `if/else` checks, early error returns, or loop exits, the programmer must remember to manually release every acquired resource before *every single exit path*:

* If you return early on an error and forget to call `free()` or `close()`, you cause a **resource/memory leak**.


* If you unlock a mutex in 4 out of 5 return branches but miss the 5th, your entire multithreaded process encounters a **deadlock**.


* In C, developers often resorted to error-prone `goto cleanup;` spaghetti patterns at the bottom of functions to centralize teardown logic.

**Your Task:**
If you were the language engineer designing Go from scratch, how would you design a language-level mechanism to guarantee that cleanup actions (like closing a file descriptor or releasing a lock) *always* execute when a function exits—regardless of whether the function returns normally at the end, returns early on an error, or crashes—without forcing the programmer to duplicate cleanup code at every single `return` statement? What naive approach would you take, and where do you think it would break?

*I am waiting for your answer.*
