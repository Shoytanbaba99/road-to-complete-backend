### Phase 1: The Generation Trap

**The Core Problem:**
Every non-trivial program runs across distinct, mutating environments: local developer workstations, continuous integration (CI) test runners, staging servers, and production multi-tenant cloud clusters.

In your Task Manager code, the storage location (`~/.tasks.json`) is hardcoded directly into the compiled machine code. If an engineer wants to:

1. Run automated integration tests on a temporary isolated file path (like `/tmp/test_tasks.json`) without destroying their personal home directory tasks;
2. Run the application in a restricted headless container where there is no user home directory (`/root` or `/home` does not exist or is read-only);
3. Switch runtime logging verbosity (e.g., `--verbose`, `--debug`), or override configuration dynamically per invocation;

...they cannot do so without modifying the `.go` source code and invoking the Go compiler to emit a new binary every single time.

A program must be configurable at **runtime launch** without recompilation. Operating systems provide two primary interfaces into an executing process's memory layout for configuration:

- The Process Argument Vector (`argv` / `os.Args`): Positional tokens passed on execution.

- The Environment Block (`environ` / `os.Environ()`): Key-value string pairs inherited from the parent process during the `execve` system call.

**Your Task:**
If you were the systems engineer tasked with designing a runtime configuration layer from scratch—allowing your binary to accept configuration via both CLI flags (e.g., `--file=/tmp/tasks.json --verbose=true`) and Environment Variables (e.g., `TASKS_FILE=/tmp/tasks.json`):

1. How would you design the parsing mechanism to handle both sources?
2. When the same configuration option is defined in **both** an Environment Variable and a Command-Line Flag simultaneously, which one should win, and why?
3. What naive approach would you take to parse flags from `os.Args` manually, and where does that naive parsing break when users pass complex inputs?

_I am waiting for your answer._
