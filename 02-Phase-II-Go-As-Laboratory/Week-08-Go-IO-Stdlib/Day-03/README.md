# Week 8 - Day 3: Go I/O & Standard Library — CLI Flags & Environment Configuration

---

## 📋 Objectives
- [x] Parse command-line flags using the standard `flag` package (`flag.StringVar`, `flag.BoolVar`, `flag.Parse()`)
- [x] Read system environment variables using `os.Getenv()` and `os.Environ()`
- [x] Establish a 3-tier configuration hierarchy (CLI Flags > Environment Variables > Defaults)
- [x] Build the hands-on **Configurable Task Manager Lab (`go_day9`)**

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of `os.Getenv()`, `flag` package flags, and configuration precedence hierarchies. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming 3-tier configuration cascading (Flags > Env Vars > Defaults). |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on Go `flag` and `os` configuration packages. |
| 🛠️ [**`learning-materials/go_day9/main.go`**](learning-materials/go_day9/main.go) | **Capstone Lab:** `AppConfig` struct, 3-tier `LoadConfig()` precedence solver, verbose debugging mode, and dynamic storage path binding. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Configuration Architecture — CLI Flags, Environment Variables & Precedence Layers]]` in `Engineers-Playbook/02 Permanent/`
