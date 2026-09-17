# Week 8 - Day 6: Go I/O & Standard Library — Benchmarks & Fuzzing

---

## 📋 Objectives
- [x] Write micro-benchmarks with `func BenchmarkXxx(b *testing.B)` using modern Go 1.24+ `b.Loop()`
- [x] Profile execution latency and heap allocations via `go test -bench=. -benchmem`
- [x] Write native coverage-guided fuzz tests with `func FuzzXxx(f *testing.F)`
- [x] Test property invariants and seed corpuses with `f.Add` and `f.Fuzz` on the **Slugify Lab (`go_lab`)**

---

## 🗺️ Day 6 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of `*testing.B`, `*testing.F`, speed/allocation metrics, and mutation testing. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming benchmark allocation profiling and native fuzzing architecture. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on Go benchmarking and fuzzing flags. |
| 🛠️ [**`go_lab/slug.go`**](go_lab/slug.go) | **Domain Utility:** URL slugifier implementation. |
| ⚡ [**`go_lab/slug_test.go`**](go_lab/slug_test.go) | **Performance & Fuzz Suite:** Unit tests, `BenchmarkSlugify` with `b.Loop()`, and `FuzzSlugify` verifying casing and space invariants. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Performance & Fuzzing — testing.B, testing.F & Invariant Verification]]` in `Engineers-Playbook/02 Permanent/`
