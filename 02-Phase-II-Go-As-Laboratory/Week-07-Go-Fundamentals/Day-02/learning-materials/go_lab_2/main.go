package main

import (
	"errors"
	"fmt"
)

func TraceLifecycle(name string) (result int, err error) {
	fmt.Printf("[1] Entering the function %s \n", name)

	defer func() {
		fmt.Printf("[5] Defer #1 executed (LIFO END). Final result value: %d \n", result)
	}()

	var zeroInt int
	var zeroStr string
	var zeroBool bool
	fmt.Printf("[2] Zero values -> int: %d, string: %q, bool %t \n", zeroInt, zeroStr, zeroBool)

	if computedVal, err := computePositive(10, 5); err == nil {
		fmt.Printf("[3] computePositive succeeded: %d\n", computedVal)
		result = computedVal
	}
	defer func() {
		fmt.Println("[4] Defer #2 executed (LIFO Start). Mutating named return value 'result'...")
		result = result + 100
	}()

	return result, nil
}

func computePositive(a, b int) (int, error) {
	if a < 0 || b < 0 {
		return 0, errors.New("negative numbers not allowed")
	}
	return a + b, nil
}

func main() {
	Transaction_Engine()
}
