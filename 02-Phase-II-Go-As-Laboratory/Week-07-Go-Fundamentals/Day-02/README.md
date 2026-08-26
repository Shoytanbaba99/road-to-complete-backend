# Week 7 - Day 2: Go Fundamentals — Variables, Constants, Functions, Control Flow & Defer

---

## 📋 Objectives
- [x] Declare variables, constants (`const MaxLimit`), and explicit types in Go
- [x] Implement multi-return functions with error handling and named return values
- [x] Master predicate `switch` statements and conditional branching
- [x] Guarantee resource cleanup and audit logging using `defer` LIFO semantics
- [x] Build the hands-on **Transaction Engine & Resource Manager Lab (`go_lab_2`)**

---

## 🗺️ Day 2 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of Go control flow, `defer` LIFO order, and state mutation. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference detailing `defer` stack execution and predicate `switch` patterns. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on Go control flow, constants, and defer mechanics. |
| 🛠️ [**`learning-materials/go_lab_2/transaction_engine.go`**](learning-materials/go_lab_2/transaction_engine.go) | **Capstone Lab:** Transaction processing engine with fee calculations, predicate `switch`, and deferred audit logging. |
| 🛠️ [**`learning-materials/go_lab_2/resourcemanager.go`**](learning-materials/go_lab_2/resourcemanager.go) | **Capstone Lab:** Resource manager guaranteeing `defer res.Close()` cleanup under simulated failure paths. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Control Flow, Multi-Return Functions & Defer Resource Cleanup]]` in `Engineers-Playbook/02 Permanent/`
