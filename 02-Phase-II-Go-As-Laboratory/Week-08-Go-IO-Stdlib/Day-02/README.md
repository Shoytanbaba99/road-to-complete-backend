# Week 8 - Day 2: Go I/O & Standard Library — JSON Encoding/Decoding, Struct Tags & Validation Boundaries

---

## 📋 Objectives
- [x] Master JSON stream processing (`json.NewDecoder` / `json.NewEncoder`) vs. in-memory processing (`json.Unmarshal` / `json.Marshal`)
- [x] Configure field mapping and omission using struct tags (`json:"key,omitempty"`)
- [x] Implement domain validation boundaries (`Validate()`) following JSON decoding
- [x] Build the hands-on **JSON Persistent Store Task Tracker Lab (`go_day8`)**

---

## 🗺️ Day 2 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal synthesis of `json.Unmarshal` vs. `json.NewDecoder` streaming and `json.Valid`. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Technical reference diagramming stream encoding/decoding and struct tag mappings. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook documentation on Go `encoding/json` package and struct tag semantics. |
| 🛠️ [**`learning-materials/go_day8/main.go`**](learning-materials/go_day8/main.go) | **Capstone Lab:** `Task` struct tags, `PersistentStore` streaming `~/.tasks.json`, domain `Validate()` method, and auto-save shutdown hook. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Go JSON Processing — Struct Tags, Stream Encoders & Validation Boundaries]]` in `Engineers-Playbook/02 Permanent/`
