package main

import (
	"bufio"
	"context"
	"fmt"
	"io"
	"strconv"
)

type LogEntry struct {
	Method     string
	URL        string
	StatusCode int
	DurationMS int64
}

type Stats struct {
	TotalRequests    int            `json:"total_requests"`
	StatusCodeCounts map[string]int `json:"status_code_counts"`
	AvgDurationMS    float64        `json:"avg_duration_ms"`
	MaxDurationMS    int64          `json:"max_duration_ms"`
	totalDurationMS  int64
}

func NewStats() *Stats {
	return &Stats{
		StatusCodeCounts: make(map[string]int),
	}
}

func (s *Stats) AddEntry(entry *LogEntry) {
	s.TotalRequests++
	s.StatusCodeCounts[strconv.Itoa(entry.StatusCode)]++
	s.totalDurationMS += entry.DurationMS
	if entry.DurationMS > s.MaxDurationMS {
		s.MaxDurationMS = entry.DurationMS
	}
	s.AvgDurationMS = float64(s.totalDurationMS) / float64(s.TotalRequests)
}

func ParseLine(line string) (*LogEntry, error) {
	var entry LogEntry

	n, err := fmt.Sscanf(line, "%s %s %d %d", &entry.Method, &entry.URL, &entry.StatusCode, &entry.DurationMS)
	if err != nil {
		return &LogEntry{}, fmt.Errorf("failed to parse line: %v", err)
	}
	if n != 4 {
		return &LogEntry{}, fmt.Errorf("invalid log entry format")
	}
	return &entry, nil
}

func ProcessStream(ctx context.Context, r io.Reader) (*Stats, error) {
	stats := NewStats()
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		if err := ctx.Err(); err != nil {
			return stats, err
		}
		line := scanner.Text()
		entry, err := ParseLine(line)
		if err != nil {
			continue // Skip malformed lines
		}
		stats.AddEntry(entry)
	}
	if err := scanner.Err(); err != nil {
		return stats, fmt.Errorf("error reading from stream: %v", err)
	}
	return stats, nil
}
