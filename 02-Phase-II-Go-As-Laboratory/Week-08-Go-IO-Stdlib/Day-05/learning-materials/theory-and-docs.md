Pull up your chair, let’s look at the screen together.

Today is **Week 8, Day 5: The `testing` Package and Table-Driven Tests**.

Up until yesterday, how were we testing code? We wrote a `main()` function, fed in some values, ran `go run main.go`, manually eyeballed the terminal output, and nodded.

Think about why that falls apart fast. You change one line inside a helper function, run your program once, and think it works. But did you check if negative numbers break? Did you check what happens if someone passes an empty string? Did you check a 10,000-character input?

No. Because running 15 variations manually by typing `go run` over and over makes you want to smash your keyboard.

So developers get lazy, check only the "happy path", ship to production, and then the server crashes at 3:00 AM because an edge case blew up the logic.

---

### The Physical Analogy: The Coin Sorting Machine

Imagine you work at a factory building a mechanical coin sorter.

You could take one coin out of your pocket, drop it in, watch it land in the right slot, and say, _"Cool, it works!"_

Or, you take a wooden tray with 50 pre-labeled slots:

- Slot 1: A brand new shiny 25-cent quarter.
- Slot 2: A scratched, beaten-up 10-cent dime.
- Slot 3: A bent 5-cent nickel.
- Slot 4: A plastic arcade token (which should be rejected immediately).

Each slot has a label on it telling you exactly what the coin is, and an engraved mark showing which bucket it _must_ drop into. You slide the tray, drop them down one by one, and in 3 seconds flat, you know if your machine handled every coin properly.

That wooden tray is a **Table-Driven Test**.

---

### How Go Solves This: The `testing` Package

Go doesn't use complex third-party testing frameworks with weird English-like assertions (`expect(x).to.be.equal(...)`).

Go’s creators took a radical approach: **Testing in Go is just normal Go code.**

Three hard-and-fast rules to memorize:

1. Your test file must end with `_test.go` (e.g., `parser_test.go`).
2. Your test function must start with `Test` followed by a capitalized word: `func TestParseInput(t *testing.T)`.
3. It takes one parameter: `t *testing.T`. This pointer gives you methods like `t.Errorf()` to report failures and `t.Fatalf()` to abort immediately.

Let’s write an actual example. Say we have a simple function that parses user role permissions for a backend:

```go
// role.go
package auth

// NormalizeRole trims input and maps it to a standard role string.
func NormalizeRole(input string) string {
    switch input {
    case "admin", "ADMIN", " Administrator ":
        return "admin"
    case "user", "USER":
        return "user"
    default:
        return "guest"
    }
}

```

Now, we could write five separate functions to test this. But that is tedious. Instead, we build the "wooden tray"—a slice of anonymous structs:

```go
// role_test.go
package auth

import "testing"

func TestNormalizeRole(t *testing.T) {
    // 1. The Table: A slice of anonymous structs holding our test inputs and expectations
    tests := []struct {
        name     string // What scenario are we checking?
        input    string // The input to feed in
        expected string // What we expect back
    }{
        {
            name:     "exact lowercase admin",
            input:    "admin",
            expected: "admin",
        },
        {
            name:     "uppercase user",
            input:    "USER",
            expected: "user",
        },
        {
            name:     "unrecognized role becomes guest",
            input:    "hacker",
            expected: "guest",
        },
        {
            name:     "empty string defaults to guest",
            input:    "",
            expected: "guest",
        },
    }

    // 2. The Runner: Loop through our tray
    for _, tc := range tests {
        // t.Run splits each case into an isolated subtest!
        t.Run(tc.name, func(t *testing.T) {
            got := NormalizeRole(tc.input)

            if got != tc.expected {
                // Notice: %q prints quotes around strings so whitespace bugs can't hide!
                t.Errorf("NormalizeRole(%q) = %q; want %q", tc.input, got, tc.expected)
            }
        })
    }
}

```

Notice what happens here?

- If 3 test cases pass and 1 fails, Go won’t stop. It finishes all of them and prints the exact name of the one that failed.
- If you find a new bug tomorrow, you don't write a new function. You just append one curly-brace line to the `tests` slice. Done.

To run it, open your terminal and hit:

```bash
go test -v ./...

```

The `-v` stands for "verbose"—it prints every single subtest name and its pass/fail state.

---

### The Beginner Trap: The Loop Variable Closure Trap

Look closely at this loop:

```go
for _, tc := range tests {
    t.Run(tc.name, func(t *testing.T) {
        // ... using tc ...
    })
}

```

In older versions of Go (prior to Go 1.22), the variable `tc` was reused across loop iterations. If you ever added `t.Parallel()` inside `t.Run`, every single test case would execute using the _last_ item in the slice!

Even though Go 1.22+ fixed loop scoping, professional Go engineers still treat test data with explicit discipline:

1. **Always print `got` vs `want**`: Never just write `t.Errorf("failed")`. Tell the reader: _What went in? What came out? What was expected?_
2. **Always format strings with `%q**`: If `got`is`"admin "`(with a trailing space) and`want`is`"admin"`, a standard `%s` looks identical in the console (`admin != admin`). `%q`prints`"admin "`vs`"admin"`. It saves hours of head-scratching.

---

### Today's Micro-Capstone

Let's put your hands on the keyboard right now.

**Objective:**
Write a small string sanitization utility `slug.go` that takes a blog or ticket title (e.g., `"Hello World!"`) and transforms it into a URL-friendly slug (`"hello-world"`). Then, write an exhaustive table-driven test in `slug_test.go` to prove it works under all conditions.

**Function signature:**

```go
func MakeSlug(title string) string

```

- Rules:
- Converts uppercase letters to lowercase.
- Replaces spaces with a single dash `-`.
- Drops special characters like `!`, `?`, and `@`.

**Acceptance Criteria:**

1. Create a table-driven test using `[]struct` containing at least **5 distinct test scenarios** (basic title, all caps, punctuation-heavy, numbers included, and pre-dashed words).
2. Use `t.Run(tc.name, ...)` for each scenario so each runs as a named subtest.
3. Verify all test scenarios pass cleanly with `go test -v`.

**Edge Case to Handle:**

- **Multiple consecutive spaces:** An input like `"Go    Standard    Library"` must produce `"go-standard-library"`, **not** `"go----standard----library"`. Add a specific test case in your table verifying this doesn't create duplicate dashes.
