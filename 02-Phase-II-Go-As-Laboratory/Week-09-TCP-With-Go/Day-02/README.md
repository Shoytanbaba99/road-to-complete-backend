# Week 9 - Day 2: TCP with Go — Read/Write Bytes, The Framing Problem & Newline-Delimited Protocols

---

## 📋 Objectives
- [x] Understand the TCP stream abstraction: why TCP is a byte stream without message boundaries
- [x] Grasp packet coalescing (Nagle's algorithm) and fragmentation (the Framing Problem)
- [x] Compare message framing strategies: delimiters (newlines), length-prefixed headers, and fixed-size records
- [x] Implement a newline-delimited request/response protocol over `net.Conn`
- [x] Use `bufio.Scanner` to effortlessly tokenize streaming lines without manual buffer slicing
- [x] Build the hands-on **Command Server (`UPPER`, `REVERSE`, `QUIT`) Lab (`go lab`)**

---

## 🗺️ Day 2 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | Personal mental model on packet coalescing, framing techniques (delimiter, length-prefix, fixed-size), and `bufio.Scanner` over `net.Conn`. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | In-depth theory covering stream framing, partial reads/writes, and protocol delimiting. |
| 🛠️ [**`learning-materials/go lab/main.go`**](learning-materials/go%20lab/main.go) | **Hands-On Server:** TCP listener on `127.0.0.1:9999`, newline protocol handling via `bufio.NewScanner(conn)`, command dispatching (`UPPER`, `REVERSE`, `QUIT`), and UTF-8 rune reversing. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[TCP Framing & Delimited Protocols — Stream Boundaries & bufio.Scanner]]` in `Engineers-Playbook/02 Permanent/`
