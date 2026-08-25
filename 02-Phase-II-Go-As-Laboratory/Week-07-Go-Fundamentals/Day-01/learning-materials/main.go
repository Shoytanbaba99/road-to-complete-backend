package main

import "fmt"

func main() {
	var msg string = "Go Toolchain Active"
	// Incorrect format verb: %s expects an integer, msg is a string
	fmt.Printf("System Status: %s\n", msg)
}
