# Day 6 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Linux Environment Variables, `execve(path, argv, envp)` inheritance, `PATH` resolution mechanics, Shebang (`#!`) interpreter loaders, Shell Startup Files (`.bashrc`/`.profile`), and Environment Hardening.

---

## 🌐 Environment Inheritance & System Call Execution Flow

```text
[ PARENT PROCESS (Bash Shell) ]
├── Local Variables (SHELL_ONLY="val")      ──► Kept internal to shell
└── Exported Variables (export ENV_KEY="val") ──► Added to envp table
            │
            ▼ (System Call: execve("/bin/deploy.sh", argv, envp))
[ CHILD PROCESS (deploy.sh) ]
├── Inherits exact copy of parent's envp table
├── Can read $ENV_KEY
└── Attempting export CHILD_VAR="val"  ──(Top-Down Isolation)──X CANNOT Mutate Parent!
```

---

## 1. Environment Variable Architecture & `execve()`
- Environment variables are key-value strings stored in a null-terminated array of pointers (`char **environ`) passed to `execve(path, argv, envp)`.
- **Top-Down Inheritance:** When a parent process spawns a child process via `fork()` + `execve()`, the child inherits a copy of the parent's exported environment array.
- **Process Isolation:** Memory mutations in the child process's environment table never propagate upward to the parent process.

---

## 2. Local Variables vs. Exported Variables
- **Local Shell Variable (`VAR="value"`):** Stored only in the shell process's internal symbol table. Not included in `envp` passed to child processes.
- **Exported Environment Variable (`export VAR="value"`):** Added to the process's `environ` array, ensuring all future child processes spawned via `execve()` inherit the variable.

---

## 3. `PATH` Resolution Mechanics & Hijacking Vulnerabilities
- The `PATH` environment variable contains a colon-separated list of directories (`/usr/bin:/bin:/usr/local/bin`).
- When executing a command without an explicit path (`ls`), the shell searches `PATH` directories **left-to-right** for the first executable match.
- **`PATH` Prepending (`export PATH="/tmp:$PATH"`):** Prepending a directory allows custom binaries to shadow standard binaries, introducing security vulnerabilities if an untrusted directory is prepended.

---

## 4. Shebang (`#!`) Interpreter Loader Mechanics
- A file starting with the magic bytes `#!` (0x23 0x21) signals the kernel ELF/script loader to invoke an interpreter binary.
- `#!/bin/bash` forces the kernel to execute `/bin/bash` with the script path as an argument, ignoring `$PATH` resolution.
- `#!/usr/bin/env bash` uses `/usr/bin/env` to locate `bash` via `$PATH`, making scripts portable across different OS layouts (e.g. Linux vs macOS vs FreeBSD).

---

## 5. Shell Startup Files & Initialization
- **Interactive Login Shell:** Reads `/etc/profile` $\rightarrow$ `~/.bash_profile` (or `~/.profile`).
- **Interactive Non-Login Shell (Subshell / Terminal tab):** Reads `~/.bashrc`.
- **Environment Hardening (`secure_wrapper.sh`):** Stripping sensitive environment keys (`unset SENSITIVE_KEY`), resetting `PATH` to safe defaults (`export PATH="/usr/bin:/bin"`), and replacing the wrapper process image via `exec`.
