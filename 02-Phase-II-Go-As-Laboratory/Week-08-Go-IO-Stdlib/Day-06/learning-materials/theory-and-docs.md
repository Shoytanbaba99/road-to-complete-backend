Today is **Week 8, Day 6: Benchmarks and Fuzzing**.

Yesterday, you proved that your code is **correct** for specific, known inputs using table-driven tests. Today, we answer two completely different engineering questions:

1. **How fast and memory-efficient is it?** (Benchmarks)
2. **Does it break when fed completely random, hostile garbage we never anticipated?** (Fuzzing)

---

### Part 1: Benchmarks (`testing.B`)

#### The Problem It Solves

Suppose you write two versions of a function that concatenates strings:

- Version A uses `+` (`s += str`).
- Version B uses `strings.Builder`.

Both pass yesterday's table-driven unit tests. Both produce the exact same output. But in production under 10,000 requests per second, Version A might choke your CPU and trash your RAM by constantly reallocating memory.

A benchmark measures:

- How many nanoseconds a single operation takes (**ns/op**).
- How many bytes of heap memory it allocates (**B/op**).
- How many distinct heap allocations it triggers (**allocs/op**).

#### How It Works: The Stopwatch Loop

In Go, a benchmark function:

1. Lives inside the same `*_test.go` file.
2. Must start with the name `Benchmark` (e.g., `BenchmarkMakeSlug`).
3. Takes a single pointer: `b *testing.B`.

Go’s runtime controls an integer called `b.N`. It runs your code in a loop, automatically increasing `b.N` until the benchmark runs long enough (usually 1 second) to get a statistically stable measurement.

```go
// slug_test.go
package main

import "testing"

func BenchmarkMakeSlug(b *testing.B) {
    input := "Hello World! This Is A Test Slug."

    // ResetTimer ignores any setup overhead above this line
    b.ResetTimer()

    // Run the loop b.N times
    for i := 0; i < b.N; i++ {
        MakeSlug(input)
    }
}

```

#### Running and Reading Benchmarks

To run your benchmarks from the terminal:

```bash
go test -bench=. -benchmem

```

- `-bench=.`: Runs all benchmark functions in the current package.
- `-benchmem`: Prints memory allocation statistics alongside execution speed.

The terminal will spit out a line like this:

```text
BenchmarkMakeSlug-8    5231940    228.4 ns/op    48 B/op    2 allocs/op

```

Breaking that down:

- **`BenchmarkMakeSlug-8`**: Ran with `GOMAXPROCS=8` (8 CPU threads available).
- **`5231940`**: The function ran over 5.2 million times during the test.
- **`228.4 ns/op`**: Each call took on average 228.4 nanoseconds.
- **`48 B/op`**: Each call allocated 48 bytes of heap memory.
- **`2 allocs/op`**: Each call touched the heap allocator 2 times.

If you optimize your code later and reduce `allocs/op` from `2` to `0`, you drastically reduce garbage collector pressure.

---

### Part 2: Fuzzing (`testing.F`)

#### The Physical Analogy: The Cat on the Keyboard

Table-driven tests only test the cases **you can think of**. Human imagination is biased toward reasonable inputs: `"admin"`, `"John Doe"`, `""`.

Now imagine an angry toddler or a cat walking across your keyboard while hitting raw UTF-8 sequences, null bytes (`\x00`), 20-megabyte strings, and broken surrogate pairs. Or worse, an attacker deliberately crafting payload bytes to trigger an out-of-bounds index panic and crash your server.

**Fuzzing** is automated chaos testing. The Go runtime generates thousands of mutated byte inputs per second, feeds them into your code, and watches for one thing: **a panic, an infinite loop, or a memory violation.**

#### How It Works: The Mutation Engine

Go includes native fuzz testing directly in the standard toolchain:

1. Function name starts with `Fuzz` (e.g., `FuzzMakeSlug`).
2. Takes a pointer: `f *testing.F`.
3. You provide a **seed corpus** (a few realistic starting points using `f.Add`).
4. You invoke `f.Fuzz` with a worker function that accepts the mutated arguments.

```go
// slug_test.go
package main

import "testing"

func FuzzMakeSlug(f *testing.F) {
    // 1. Seed Corpus: Give Go valid starting examples to mutate
    f.Add("Hello World")
    f.Add("admin-panel-2026")
    f.Add("!@#$%^&*()")
    f.Add("")

    // 2. The Fuzz Target: Go mutates 'input' relentlessly
    f.Fuzz(func(t *testing.T, input string) {
        // Run the function with the random input
        result := MakeSlug(input)

        // Rule 1: It must never crash/panic. (Go catches panics automatically)

        // Rule 2: Invariant Check.
        // A slug should NEVER contain uppercase letters, regardless of input.
        for _, r := range result {
            if r >= 'A' && r <= 'Z' {
                t.Fatalf("MakeSlug(%q) produced uppercase character %c in %q", input, r, result)
            }
        }
    })
}

```

#### Running the Fuzz Test

To start the fuzzer, tell Go how long to run it:

```bash
go test -fuzz=FuzzMakeSlug -fuzztime=10s

```

Go runs continuous iterations in parallel across your CPU cores:

```text
fuzz: elapsed: 0s, execs: 0 (0/sec)
fuzz: elapsed: 3s, execs: 412984 (137648/sec)
fuzz: elapsed: 6s, execs: 890123 (159046/sec)
PASS

```

If the fuzzer finds an input that panics or fails your assertion:

- It stops immediately.
- It saves the exact failing byte string into a file in `testdata/fuzz/FuzzMakeSlug/...`.
- The next time you run plain `go test`, Go automatically includes that failing input as a standard test case so the bug never regresses.

---

### Summary Checklist

- **Unit Tests (`*testing.T`)**: Verify expected outputs for specific known inputs.
- **Benchmarks (`*testing.B`)**: Measure runtime speed (`ns/op`) and heap allocations (`B/op`, `allocs/op`). Run with `go test -bench=. -benchmem`.
- **Fuzz Tests (`*testing.F`)**: Automatically bombard the function with generated edge cases to hunt for panics and broken invariants. Run with `go test -fuzz=...`.

---

### Today's Micro-Capstone

Let's test the durability and speed of your slugifier from Day 5.

**Objective:**
Take your `MakeSlug(title string) string` implementation and add a benchmark and a fuzz test to `slug_test.go`.

**Tasks:**

1. **Write `BenchmarkMakeSlug(b \*testing.B)**`:

- Measure a realistic 5-to-10-word sentence.
- Run `go test -bench=. -benchmem` and record your `ns/op` and `allocs/op`.

2. **Write `FuzzMakeSlug(f \*testing.F)**`:

- Seed it with at least 3 distinct strings.
- Inside `f.Fuzz`, check two invariants:
- No uppercase characters exist in the output.
- No spaces `' '` exist in the output.

- Run the fuzzer for 10 seconds with `go test -fuzz=FuzzMakeSlug -fuzztime=10s`. Ensure it reports `PASS` without triggering an index out-of-range panic.
