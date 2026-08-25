# Week 7 - Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Environment, Go Modules (`go.mod`), Package Scope, and Toolchain Commands (`go run/build/test/fmt/vet/env`).

---

## 🌐 Go Toolchain & Module Execution Lifecycle

```text
[ SOURCE DIRECTORY ]
  main.go (package main) ──► go.mod (module name & deps)
  ├── go fmt  ──► Formats code canonically (gofmt rules)
  ├── go vet  ──► Static analysis (detects suspicious bugs & unhandled errors)
  ├── go test ──► Executes main_test.go unit tests
  ├── go run  ──► Compiles temp binary & executes in-memory
  └── go build──► Emits standalone native executable binary (e.g. ./basics_bin)
```

---

## 1. Core Go Environment Variables (`go env`)

| Environment Variable | Role in Go Toolchain |
|---|---|
| **`GOROOT`** | Directory path where Go standard library & compiler tools are installed. |
| **`GOPATH`** | Workspace directory for cached downloaded modules (`$GOPATH/pkg/mod`) and installed binaries (`$GOPATH/bin`). |
| **`GOOS`** | Target Operating System for cross-compilation (e.g., `linux`, `darwin`, `windows`). |
| **`GOARCH`** | Target CPU Architecture for cross-compilation (e.g., `amd64`, `arm64`). |
| **`CGO_ENABLED`** | Enables/disables C bindings (`CGO_ENABLED=0` produces static pure-Go binaries). |

---

## 2. Essential Go Toolchain Commands

- **`go mod init <name>`:** Initializes a new Go module and generates `go.mod`.
- **`go fmt ./...`:** Formats code across all project packages according to canonical Go standards.
- **`go vet ./...`:** Analyzes source code for structural flaws, unreachable code, and Printf argument mismatches.
- **`go build -o app`:** Compiles code into a optimized native executable binary.
