package main

import "testing"

func TestAssertion(t *testing.T) {
	expected := 42
	actual := 40 + 2
	if actual != expected {
		t.Fatalf("Expected %d, got %d", expected, actual)
	}
}
