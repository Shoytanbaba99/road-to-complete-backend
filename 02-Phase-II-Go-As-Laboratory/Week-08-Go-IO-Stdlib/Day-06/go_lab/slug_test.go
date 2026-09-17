package golab

import (
	"testing"
)

func TestSlugify(t *testing.T) {
	tests := []struct {
		name     string
		input    string
		expected string
	}{
		{
			name:     "Basic test",
			input:    "Hello World",
			expected: "hello-world",
		},
		{
			name:     "Test with special characters",
			input:    "Hello !World!",
			expected: "hello-world",
		},
		{
			name:     "Test with multiple spaces",
			input:    "Hello   World",
			expected: "hello-world",
		},
		{
			name:     "Test with mixed case",
			input:    "HeLLo WoRLd",
			expected: "hello-world",
		},
	}
	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			result := Slugify(tc.input)
			if result != tc.expected {
				t.Errorf("Slugify(%q) = %q, want %q", tc.input, result, tc.expected)
			}
		})
	}
}

func BenchmarkSlugify(b *testing.B) {
	input := "Hello World! This Is A Test Slug."

	for b.Loop() {
		Slugify(input)
	}
}

func FuzzSlugify(f *testing.F) {
	// Seed the fuzzer with some initial inputs
	f.Add("Hello World")
	f.Add("Hello !World!")
	f.Add("Hello   World")

	f.Fuzz(func(t *testing.T, a string) {
		result := Slugify(a)
		for _, r := range result {
			if r >= 'A' && r <= 'Z' {
				t.Errorf("Slugify(%q) = %q, contains uppercase letter %q", a, result, r)
			}
			if r == ' ' {
				t.Errorf("Slugify(%q) = %q, contains space", a, result)
			}
		}
	})
}
