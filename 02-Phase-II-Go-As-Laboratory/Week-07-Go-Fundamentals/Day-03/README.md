# Week 7 - Day 3: Go Fundamentals — Arrays, Slices, Maps, Strings, Runes & Bytes

---

## 📋 Objectives
- [x] Understand fixed-size arrays `[N]T` vs. dynamic slice headers `[]T` (`pointer`, `len`, `cap`)
- [x] Initialize and operate on hash maps `map[K]V` (`make()`, key checks, nil map safety)
- [x] Differentiate between raw bytes (`byte`/`uint8`), strings, and Unicode code points (`rune`/`int32`)
- [x] Build the hands-on **Word Counter & UTF-8 Text Analyzer Lab (`simple_text.go`)**

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of slice headers, map initialization rules, and string vs rune UTF-8 byte counting. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference detailing slice memory headers and UTF-8 string encoding layouts. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on slice backing arrays, hash map buckets, and Unicode runes. |
| 🛠️ [**`learning-materials/day3_lab/simple_text.go`**](learning-materials/day3_lab/simple_text.go) | **Capstone Lab:** Word frequency counter and UTF-8 `utf8.RuneCountInString` vs. raw byte length analyzer. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Memory Layout — Slices, Maps, Strings & Unicode Runes]]` in `Engineers-Playbook/02 Permanent/`
