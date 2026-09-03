---

### Phase 1: The Generation Trap

**The Core Problem:**

Imagine a standard, single-threaded backend CLI tool or request processor. There are no background threads, no goroutines, and no parallel workers. Everything executes sequentially down a deep chain of standard function calls:

```text
main()
  └── ProcessCommand()
        └── LoadDataFromNetworkOrDisk()
              └── ReadChunks()
                    └── ParseBytes()

```

Every function in this call stack takes time. `LoadDataFromNetworkOrDisk()` might take 500 milliseconds, `ReadChunks()` might take another 300 milliseconds, and `ParseBytes()` might take 200 milliseconds.

Now, consider two very common operational constraints:

1. **The Call-Stack Budget (Deadlines):**
The user specifies at the CLI: `--timeout=200ms`. The entire execution—from `main()` all the way through the deepest nested function—must complete within 200 milliseconds. If `LoadDataFromNetworkOrDisk()` alone eats 180 milliseconds, the downstream functions (`ReadChunks`, `ParseBytes`) must be aware that they only have 20 milliseconds left to finish before they must immediately abort and return an error without wasting CPU time or disk I/O on work that is already doomed to fail.
2. **The Mid-Operation Signal (Cancellation):**
While `LoadDataFromNetworkOrDisk()` is halfway through running a loop reading 1,000 blocks, a condition occurs higher up the call stack (or an OS interrupt signal arrives, like `SIGINT` / `Ctrl+C`) signaling that this work is no longer wanted. The caller needs a standard way to propagate an "ABORT NOW" instruction down through five layers of nested function calls so that the lowest-level loop immediately stops reading, returns an error, and unrolls the call stack.

In traditional languages, how do people solve this?

* They pass a bunch of disconnected arguments into *every single function*: `func ParseBytes(data []byte, startTime time.Time, timeout time.Duration, isAborted *bool)`.
* Every developer invents their own ad-hoc struct for timeouts.
* Functions in third-party packages have no universal standard for knowing when they should stop executing.

---

**Your Task:**

Do not mention `goroutines`, `channels`, or `context.Context`.

If you were the systems engineer tasked with designing a universal mechanism for standard, sequential Go code to solve this from scratch:

1. How would you design a way for a top-level function (`main`) to establish a time limit (a deadline) and pass it down through 5 layers of function calls, so that any function along the way can easily check whether time has run out?
2. If the user hits an abort condition at layer 2, how would you signal that down to layer 5 without modifying every function's business-logic return types or creating messy global variables?
3. What naive approach would you take, and where do you think it would break?

_I am waiting for your answer._
