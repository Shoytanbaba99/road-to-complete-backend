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
