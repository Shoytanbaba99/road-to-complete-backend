package main

import (
	"context"
	"strings"
	"testing"
)

func TestParseLine(t *testing.T) {
	tests := []struct {
		name        string
		input       string
		wantEntry   LogEntry
		expectError bool
	}{
		{
			name:  "valid standard line",
			input: "GET /api/v1/tickets 200 45",
			wantEntry: LogEntry{
				Method:     "GET",
				URL:        "/api/v1/tickets",
				StatusCode: 200,
				DurationMS: 45,
			},
			expectError: false,
		},
		{
			name:        "empty line",
			input:       "",
			wantEntry:   LogEntry{},
			expectError: true,
		},
		{
			name:        "insufficient fields",
			input:       "GET /api/v1/tickets 200",
			wantEntry:   LogEntry{},
			expectError: true,
		},
		{
			name:        "non-integer status code",
			input:       "GET /api/v1/tickets OK 45",
			wantEntry:   LogEntry{},
			expectError: true,
		},
		{
			name:  "duration with ms suffix",
			input: "GET /api/v1/tickets 200 45ms",
			wantEntry: LogEntry{
				Method:     "GET",
				URL:        "/api/v1/tickets",
				StatusCode: 200,
				DurationMS: 45,
			},
			expectError: false,
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			got, err := ParseLine(tc.input)
			if tc.expectError && err == nil {
				t.Fatalf("ParseLine(%q) expected error; got nil", tc.input)
			}
			if !tc.expectError && err != nil {
				t.Fatalf("ParseLine(%q) unexpected error: %v", tc.input, err)
			}
			if got == nil {
				if tc.wantEntry != (LogEntry{}) {
					t.Errorf("ParseLine(%q) = nil; want %+v", tc.input, tc.wantEntry)
				}
			} else if *got != tc.wantEntry {
				t.Errorf("ParseLine(%q) = %+v; want %+v", tc.input, *got, tc.wantEntry)
			}
		})
	}
}

func TestProcessStream(t *testing.T) {
	data := `GET /index.html 200 10
POST /login 401 20
GET /dashboard 200 30
MALFORMED LINE TO SKIP
GET /logout 200 40`

	r := strings.NewReader(data)
	stats, err := ProcessStream(context.Background(), r)
	if err != nil {
		t.Fatalf("ProcessStream failed: %v", err)
	}

	if stats.TotalRequests != 4 {
		t.Errorf("expected 4 total lines, got %d", stats.TotalRequests)
	}
	if stats.StatusCodeCounts["200"] != 3 {
		t.Errorf("expected 3 counts for status 200, got %d", stats.StatusCodeCounts["200"])
	}
	if stats.StatusCodeCounts["401"] != 1 {
		t.Errorf("expected 1 count for status 401, got %d", stats.StatusCodeCounts["401"])
	}
	if stats.totalDurationMS != 100 {
		t.Errorf("expected total duration 100, got %d", stats.totalDurationMS)
	}
	if stats.MaxDurationMS != 40 {
		t.Errorf("expected max duration 40, got %d", stats.MaxDurationMS)
	}
	if float64(stats.AvgDurationMS) != 25.0 {
		t.Errorf("expected avg duration 25.0, got %f", stats.AvgDurationMS)
	}
}

func BenchmarkParseLine(b *testing.B) {
	line := "POST /api/v1/checkout/orders 201 185"

	b.ResetTimer()
	for b.Loop() {
		_, _ = ParseLine(line)
	}
}

func FuzzParseLine(f *testing.F) {
	f.Add("GET /api/v1/health 200 4")
	f.Add("POST /login 500 100")
	f.Add("")
	f.Add("   ")
	f.Add("MALFORMED INVALID STRING")
	f.Fuzz(func(t *testing.T, input string) {
		_, _ = ParseLine(input)
	})
}
