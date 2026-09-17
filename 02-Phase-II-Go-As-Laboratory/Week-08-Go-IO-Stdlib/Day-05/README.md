# Week 8 - Day 5: Go I/O & Standard Library — Testing Package & Table-Driven Tests

---

## 📋 Objectives
- [x] Master standard Go testing conventions (`*_test.go`, `func TestXxx(t *testing.T)`)
- [x] Structure unit tests using table-driven test patterns (`[]struct`)
- [x] Isolate and name individual test iterations with `t.Run()`
- [x] Build and test the hands-on **Slugify String Utility Lab (`go_lab`)**

---

## 🗺️ Day 5 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of `*testing.T`, `_test.go` naming rules, and table-driven testing ergonomics. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming table-driven subtest execution and test failure paradigms. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on Go `testing` package conventions. |
| 🛠️ [**`learning-materials/go_lab/slug.go`**](learning-materials/go_lab/slug.go) | **Domain Utility:** URL slugifier with `strings.Builder`, rune lowering, and whitespace collapsing. |
| 🧪 [**`learning-materials/go_lab/slug_test.go`**](learning-materials/go_lab/slug_test.go) | **Unit Test Suite:** Table-driven test suite with `t.Run()` covering basic, punctuation, whitespace, and case variants. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Testing — Table-Driven Tests, Subtests & testing.T]]` in `Engineers-Playbook/02 Permanent/`
