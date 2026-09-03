# Week 8 - Day 4: Go I/O & Standard Library — `context.Context`, Cancellation & Deadlines

---

## 📋 Objectives
- [x] Understand `context.Context` for request lifecycles, cancellation trees, and deadlines
- [x] Create root and derived contexts using `context.Background()`, `context.WithTimeout()`, and `context.WithCancel()`
- [x] Inspect cancellation state via `<-ctx.Done()` and `ctx.Err()`
- [x] Build the hands-on **Context-Aware Report Exporter Lab (`go_day4`)**

---

## 🗺️ Day 4 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of `context.Context`, `WithTimeout`, `WithCancel`, and `ctx.Done()` / `ctx.Err()`. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming tree-based cancellation propagation and timeout error handling. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on Go `context` package. |
| 🛠️ [**`learning-materials/go_day4/main.go`**](learning-materials/go_day4/main.go) | **Capstone Lab:** `ExportReport` with timeout context cancellation check (`ctx.Err()`), CLI command `export` with `--timeout` flag, and `context.DeadlineExceeded` inspection. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Concurrency & Lifecycles — context.Context, Cancellation & Deadlines]]` in `Engineers-Playbook/02 Permanent/`
