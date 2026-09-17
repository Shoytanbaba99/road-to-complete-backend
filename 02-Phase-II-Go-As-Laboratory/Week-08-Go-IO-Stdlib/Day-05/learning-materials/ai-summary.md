# Week 8 - Day 5 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Unit Testing: `*testing.T`, Table-Driven Tests (`[]struct`), Subtests via `t.Run()`, Failure reporting (`t.Errorf` vs `t.Fatalf`), and Deterministic Test Suites.

---

## 🌐 Go Table-Driven Test Architecture

```text
[ TABLE SPECIFICATION: tests := []struct{ name, input, expected } ]
  ├── Case 1: "Basic test"                 -> "Hello World"   => "hello-world"
  ├── Case 2: "With special characters"    -> "Hello !World!" => "hello-world"
  ├── Case 3: "Multiple spaces"            -> "Hello   World" => "hello-world"
  └── Case 4: "Mixed case"                 -> "HeLLo WoRLd"   => "hello-world"
                                    │
                                    ▼
[ SUBTEST RUNNER: t.Run(tc.name, func(t *testing.T)) ]
  ├── Isolates failure boundaries per test case
  ├── Enables targeted execution: go test -run TestSlugify/Basic
  └── Clean diagnostic reporting: t.Errorf("got %q, want %q", result, tc.expected)
```

---

## 1. Core Go Testing Idioms

| Testing Concept | Syntax / Tooling | Engineering Benefit |
|---|---|---|
| **Naming Conventions** | File: `*_test.go`<br>Func: `func TestXxx(t *testing.T)` | Automated discovery by `go test ./...` with zero test-runner config. |
| **Table-Driven Tests** | Slice of anonymous structs defining inputs & expectations | Cleanly scales test coverage without duplicating assertion logic. |
| **Subtests (`t.Run`)** | `t.Run(tc.name, func(t *testing.T) { ... })` | Provides distinct failure output and allows running individual subcases. |
| **Non-Fatal Failures** | `t.Errorf(...)` | Reports an assertion failure but continues running subsequent checks in the case. |
| **Fatal Aborts** | `t.Fatalf(...)` | Immediately halts the current test function (essential for pre-condition checks like `nil` pointers). |
