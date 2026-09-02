# Week 8 - Day 1: Go I/O & Standard Library — Files, `io.Reader`/`Writer` & `bufio`

---

## 📋 Objectives
- [x] Master core stream interfaces `io.Reader` and `io.Writer`
- [x] Manage file descriptors safely with `os.Open`, `os.Create`, and `defer file.Close()`
- [x] Understand system call reduction and RAM buffering with `bufio.Writer` (`Flush()`) and `bufio.Scanner`
- [x] Build the hands-on **File Persistent REPL Task Tracker Lab (`go_day1`)**

---

## 🗺️ Day 1 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of `io.Reader`/`Writer`, system call overhead, and `bufio` RAM buffering. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming user-space RAM buffers vs kernel `sys_write` syscalls. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on Go `io` and `bufio` packages. |
| 🛠️ [**`learning-materials/go_day1/main.go`**](learning-materials/go_day1/main.go) | **Capstone Lab:** Interactive REPL shell with `bufio.NewScanner(os.Stdin)`, `SaveTasksToFile` (`bufio.Writer`), and `LoadTasksFromFile` (`bufio.Scanner`). |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Stream I/O — io.Reader, io.Writer & bufio System Call Optimization]]` in `Engineers-Playbook/02 Permanent/`
