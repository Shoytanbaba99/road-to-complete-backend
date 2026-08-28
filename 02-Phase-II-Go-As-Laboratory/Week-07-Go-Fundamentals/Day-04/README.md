# Week 7 - Day 4: Go Fundamentals — Structs, Methods, Pointers & Zero Values

---

## 📋 Objectives
- [x] Declare custom data types with `struct` and understand field zero-value defaults
- [x] Master pointer semantics (`*T`, `&x`, `*p`) and zero pointer arithmetic guarantees
- [x] Distinguish between Value Receivers `(p T)` vs. Pointer Receivers `(p *T)`
- [x] Build the hands-on **Packet Verification Engine Lab (`go_day4_lab`)**

---

## 🗺️ Day 4 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of struct memory allocation, pointer safety, and receiver semantics. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming value vs. pointer receiver memory behavior. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on struct alignment, method sets, and pointer rules. |
| 🛠️ [**`learning-materials/go_day4_lab/main.go`**](learning-materials/go_lab4/main.go) | **Capstone Lab:** `Packet` struct with constructor pattern, pointer receiver `Verify()`, and value receiver `Display()`. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go Structs, Pointer Receivers & Memory Alignment]]` in `Engineers-Playbook/02 Permanent/`
