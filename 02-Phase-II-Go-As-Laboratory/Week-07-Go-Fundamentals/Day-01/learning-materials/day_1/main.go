package main

import (
	"calculator/env"
	"calculator/operations"
	"fmt"
)

func main() {
	// Read environment variable
	mode := env.GetMode()

	// Perform calculations
	subResult := operations.Subtract(100, 25)
	mulResult := operations.Multiply(5, 5)
	addResult := operations.Add(10, 10)
	divResult := operations.Div(10, 10)

	// Output results
	fmt.Println("=== CALCULATOR CLI ===")
	fmt.Printf("OP_MODE: %s\n", mode)
	fmt.Printf("Subtract(100, 25) = %d\n", subResult)
	fmt.Printf("Multiply(5, 5) = %d\n", mulResult)
	fmt.Printf("Addition(10, 10) = %d\n", addResult)
	fmt.Printf("Division(10,10) = %d\n", divResult)
}
