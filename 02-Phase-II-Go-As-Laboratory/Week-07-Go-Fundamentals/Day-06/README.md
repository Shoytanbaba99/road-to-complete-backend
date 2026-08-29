# Week 7 - Day 6: Go Fundamentals — Error Handling: Sentinel Errors, Custom Errors & Error Wrapping

---

## 📋 Objectives
- [x] Treat errors as values satisfying the built-in `error` interface (`Error() string`)
- [x] Create static sentinel errors (`var ErrWrongPassword = errors.New(...)`)
- [x] Design custom error structs carrying rich metadata (`RateLimitError`)
- [x] Wrap errors with `fmt.Errorf("%w", err)` and inspect chains via `errors.Is()` and `errors.As()`
- [x] Build the hands-on **Authentication Error Handler Lab (`go_day6`)**

---

## 🗺️ Day 6 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of sentinel errors, custom error structs, error wrapping (`%w`), and `errors.Is`/`errors.As`. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming wrapped error chains and inspection mechanics. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on Go error handling idioms, unwrapping, and Go 1.20 `errors.Join`. |
| 🛠️ [**`learning-materials/go_day6/main.go`**](learning-materials/go_day6/main.go) | **Capstone Lab:** Auth handler with `ErrWrongPassword` sentinel, `RateLimitError` custom struct, `%w` wrapping, and `Is`/`As` checks. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Error Handling — Sentinel Errors, Wrapping & Error Chains]]` in `Engineers-Playbook/02 Permanent/`
