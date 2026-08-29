### Phase 1: The Generation Trap

**The Core Problem:**
In early software architectures and traditional languages, handling unexpected runtime failures was solved using two distinct, deeply flawed paradigms:

1. **Integer Return Codes & Global Flags (e.g., C/POSIX):** A function returns `-1` or `NULL` to indicate a failure, and sets a global thread-local integer like `errno`. The caller is left to inspect a cryptic numeric code with zero contextual awareness of where the error actually happened in the call stack, what parameters caused it, or how many internal layers deep it originated.

2. **Hidden Control-Flow Exceptions (e.g., Java, Python, C++):** Functions throw `Exception` objects that invisibly bypass standard control flow, unwinding the CPU stack across arbitrary function boundaries until caught by an enclosing `try/catch` block. This makes control flow invisible, causes hidden resource leaks when programmers forget which intermediate functions can throw, and incurs significant runtime performance overhead from stack-frame unwinding.

Furthermore, as a program grows into deep architectural layers (e.g., a low-level disk I/O error moving up through a database layer, then through a business service layer, and finally reaching an API handler), errors lose their meaning:

- If an internal function blindly replaces the low-level error with a generic message (e.g., `"failed to fetch user"`), the engineering team loses the root cause (`"connection refused on port 5432"`).
- If the function passes the raw low-level error directly to the caller, higher layers lose the architectural context of what operation was actually being attempted when it failed.
- If a higher layer needs to make a programmatic decision (e.g., "if this is a `RecordNotFound` error, return 404; if it is a `NetworkTimeout`, retry; otherwise crash"), string parsing (`strings.Contains(err.Error(), "not found")`) is brittle, unsafe, and constantly breaks during minor refactors.

Go needed a way to treat failures as explicit, first-class values that do not bypass control flow, retain the entire chain of root-cause context as they traverse upwards through call stacks, and allow callers to programmatically inspect underlying failure types without fragile string matching.

---

**Your Task:**
Do not tell me how Go implements its error system.

If you were the systems engineer tasked with designing an error-handling and failure-propagation system from scratch:

1. How would you design errors so that functions cannot secretly bypass control flow, while ensuring programmers never lose the original root cause as errors bubble up through 5 layers of function calls?
2. How would you allow an top-level caller (like an API handler) to check if an error that happened 5 layers deep was specifically a `"Resource Locked"` error, even if intermediate layers attached additional context messages to it along the way?

What naive approach would you take, and where do you think it would break?

_I am waiting for your answer._

---

In Go, an **error is just a normal variable**. Go does not have magic `try/catch` or `throw` blocks. If something goes wrong, a function simply hands you an error object as a return value.

There are **three simple patterns** you need to know:

1. **The Sentinel Error (The Fixed Constant):**

- A predefined, named error you declare at the top of your package.

- _Example:_ `var ErrNotFound = errors.New("not found")`.

- You use this when you want a fixed, recognizable error that never changes.

2. **The Wrapped Error (Adding Context to the Story):**

- If a low-level function gives you an error, you don't want to throw away what happened, but you want to add your own note to it.

- You use `fmt.Errorf("failed doing X: %w", err)`.

- The **`%w`** verb is the secret sauce: it puts the old error inside the new error like an envelope.

- If you use `%v` instead of `%w`, you crush the envelope into flat text and destroy the original error.

3. **Inspecting Wrapped Errors (`errors.Is` vs `errors.As`):**

- **`errors.Is(err, target)`:** Used for Sentinel errors. It asks: _"Is `ErrNotFound` inside this chain of envelopes anywhere?"_ Returns `true` or `false`.

- **`errors.As(err, &targetStruct)`:** Used for Custom Struct errors. It asks: _"Is there a specific custom struct inside this chain of envelopes? If yes, unpack its fields so I can read them."_

---

### Combined Lab: Empirical Proof & Deliberate Breakage

Create a single workspace and a single file to see all of this work, break it, and prove it with zero confusion.

```bash
mkdir -p ~/go_day6_simple && cd ~/go_day6_simple
go mod init simpleerror

```

Create `main.go`:

```go
package main

import (
	"errors"
	"fmt"
)

// 1. Sentinel Error (Fixed standard error)
var ErrPermissionDenied = errors.New("permission denied")

// 2. Custom Error (A struct that holds extra details)
type HttpError struct {
	StatusCode int
	Endpoint   string
}

// Any struct with this exact Error() string method becomes a Go error
func (h *HttpError) Error() string {
	return fmt.Sprintf("HTTP %d error on %s", h.StatusCode, h.Endpoint)
}

// Low-level function: returns our custom struct error
func fetchDatabase() error {
	return &HttpError{StatusCode: 500, Endpoint: "/api/v1/users"}
}

// Middle-level function: wraps the lower error with %w
func getUser() error {
	err := fetchDatabase()
	if err != nil {
		// %w wraps the inner error inside our new message
		return fmt.Errorf("getUser failed: %w", err)
	}
	return nil
}

func main() {
	err := getUser()

	fmt.Println("--- 1. Full Error Output ---")
	fmt.Println(err)

	fmt.Println("\n--- 2. Checking with errors.Is ---")
	// Check if our error contains ErrPermissionDenied
	if errors.Is(err, ErrPermissionDenied) {
		fmt.Println("Found ErrPermissionDenied!")
	} else {
		fmt.Println("ErrPermissionDenied was NOT in the error chain.")
	}

	fmt.Println("\n--- 3. Unpacking with errors.As ---")
	// Target variable to hold the extracted custom error
	var httpErr *HttpError

	// errors.As looks through the chain and fills httpErr if it finds a match
	if errors.As(err, &httpErr) {
		fmt.Println("Successfully unpacked HttpError!")
		fmt.Printf("Status Code: %d\n", httpErr.StatusCode)
		fmt.Printf("Endpoint:    %s\n", httpErr.Endpoint)
	}
}

```

Run it:

```bash
go run main.go

```

**What you will see:**

```text
--- 1. Full Error Output ---
getUser failed: HTTP 500 error on /api/v1/users

--- 2. Checking with errors.Is ---
ErrPermissionDenied was NOT in the error chain.

--- 3. Unpacking with errors.As ---
Successfully unpacked HttpError!
Status Code: 500
Endpoint:    /api/v1/users

```

---

### The 3 Breakage Drills (Break It and See Why)

##### Drill 1: Destroying the Chain with `%v` instead of `%w`

- **Break it:** In `getUser()`, change `fmt.Errorf("getUser failed: %w", err)` to use **`%v`**:

```go
return fmt.Errorf("getUser failed: %v", err)

```

- **Run:** `go run main.go`
- **Result:** `errors.As` fails completely! It prints nothing under section 3 because `%v` flattened the struct into plain text and threw away the error structure.

##### Drill 2: Forgetting the Pointer Address `&` in `errors.As`

- **Break it:** Revert back to `%w`. In `main()`, change `errors.As(err, &httpErr)` to pass `httpErr` without the `&`:

```go
errors.As(err, httpErr)

```

- **Run:** `go run main.go`
- **Result:** Instant panic! `panic: errors: *target must be interface or implement error`. `errors.As` needs a pointer to the variable so it can write the found error into it.

##### Drill 3: Returning an Unwrapped Sentinel

- **Break it:** In `fetchDatabase()`, change the return to `return ErrPermissionDenied`.
- **Run:** `go run main.go`
- **Result:** Section 2 now prints `Found ErrPermissionDenied!`, proving that even though `getUser()` added `"getUser failed: "`, `errors.Is` could still look inside and find the sentinel.

---

### The Non-Negotiable Invariant

> **The Error Invariant:** Go errors are explicit values returned from functions. When adding context to an error, you must use `fmt.Errorf` with **`%w`** to preserve the chain; otherwise, programmatic tools like `errors.Is` and `errors.As` cannot inspect the root cause.

---

### Day 6 Capstone: The Simple Login Error Checker

Build a single `main.go` file inside a fresh directory `~/day6_simple_capstone`:

1. Create a sentinel error: `var ErrWrongPassword = errors.New("incorrect password")`.

2. Create a custom error struct `RateLimitError` with one field: `WaitSeconds int`. Give it an `Error() string` method.

3. Write a function `login(attempts int, password string) error`:

- If `attempts > 3`, return a `&RateLimitError{WaitSeconds: 60}`.

- If `password != "secret"`, return `ErrWrongPassword`.

- Otherwise, return `nil`.

4. Write a function `handleLogin(attempts int, password string) error` that calls `login()` and wraps any error with `fmt.Errorf("login failed: %w", err)`.

5. In `main()`:

- Test a bad password (`handleLogin(1, "wrong")`). Use `errors.Is` to check for `ErrWrongPassword` and print `"Please try again."`

- Test too many attempts (`handleLogin(5, "secret")`). Use `errors.As` to extract the `*RateLimitError` and print `"Blocked! Wait X seconds."`

Give this a run and tell me how the output looks on your screen.
