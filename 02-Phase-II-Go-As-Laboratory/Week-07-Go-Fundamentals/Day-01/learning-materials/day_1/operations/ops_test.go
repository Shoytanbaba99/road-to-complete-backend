package operations

import "testing"

func TestMultiply(t *testing.T) {
	result := Multiply(5, 5)
	expected := 25
	if result != expected {
		t.Errorf("Multiply(5, 5) = %d; want %d", result, expected)
	}

	result = Multiply(0, 10)
	expected = 0
	if result != expected {
		t.Errorf("Multiply(0, 10) = %d; want %d", result, expected)
	}

	result = Multiply(-3, 4)
	expected = -12
	if result != expected {
		t.Errorf("Multiply(-3, 4) = %d; want %d", result, expected)
	}
}

func TestSubtract(t *testing.T) {
	result := Subtract(100, 25)
	expected := 75
	if result != expected {
		t.Errorf("Subtract(100, 25) = %d; want %d", result, expected)
	}

	result = Subtract(10, 10)
	expected = 0
	if result != expected {
		t.Errorf("Subtract(10, 10) = %d; want %d", result, expected)
	}

	result = Subtract(5, 10)
	expected = -5
	if result != expected {
		t.Errorf("Subtract(5, 10) = %d; want %d", result, expected)
	}
}
