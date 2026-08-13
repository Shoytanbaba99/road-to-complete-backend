# Week 1 - Day 6: Environment Variables, PATH Resolution & Shell Scripts

---

## 📋 Objectives
- [x] Environment variables & `execve(path, argv, envp)` key-value array
- [x] Top-down environment inheritance & child process isolation
- [x] Local shell variables vs. exported environment variables (`export`)
- [x] `PATH` resolution mechanics (left-to-right search) & `PATH` prepending security
- [x] Shebang (`#!`) interpreter loaders (`#!/bin/bash` vs `#!/usr/bin/env bash`)
- [x] Shell startup file hierarchy (`.profile`, `.bashrc`)
- [x] Build Bash deployment script (`deploy.sh`) & secure environment wrapper (`secure_wrapper.sh`)

---

## 🗺️ Day 6 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of environment inheritance, `PATH` resolution, and shell initialization. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, POSIX envp guides, and script security rules. |
| 📜 [**`learning-materials/deploy.sh`**](learning-materials/deploy.sh) | Bash deployment script validating inherited credentials (`DB_PASSWORD`) and testing `PATH` prepending. |
| 🛡️ [**`learning-materials/secure_wrapper.sh`**](learning-materials/secure_wrapper.sh) | Secure shell wrapper unsetting sensitive credentials (`unset`), resetting `PATH`, and executing target commands via `exec`. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# View raw environment array directly from kernel memory for a running process
cat /proc/[pid]/environ | tr '\0' '\n'

# Test environment inheritance with subshells
export TEST_VAR="HelloKernel"
bash -c 'echo $TEST_VAR'

# Trace binary PATH resolution
which ls
type -a cat

# Test secure environment wrapper
chmod +x secure_wrapper.sh deploy.sh
./secure_wrapper.sh env
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Environment Variables, PATH Resolution & Shell Execution]]` in `Engineers-Playbook/02 Permanent/`
