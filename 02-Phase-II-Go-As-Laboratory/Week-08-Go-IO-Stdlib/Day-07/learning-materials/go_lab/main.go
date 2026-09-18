package main

import (
	"context"
	"encoding/json"
	"flag"
	"fmt"
	"os"
	"time"
)

func main() {
	filepath := flag.String("file", "", "Path to the log file")
	timeout := flag.Int("timeout", 0, "Timeout in seconds")
	flag.Parse()

	if *filepath == "" {
		fmt.Fprintln(os.Stderr, "Error: -file flag is required")
		flag.Usage()
		return
	}
	file, err := os.Open(*filepath)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error opening file: %v\n", err)
		return
	}
	defer file.Close()
	var ctx context.Context
	var cancel context.CancelFunc

	if *timeout > 0 {
		ctx, cancel = context.WithTimeout(context.Background(), time.Duration(*timeout)*time.Second)
	} else {
		ctx, cancel = context.WithCancel(context.Background())
	}

	defer cancel()

	stats, err := ProcessStream(ctx, file)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error processing stream: %v\n", err)
		os.Exit(1)
	}

	encoder := json.NewEncoder(os.Stdout)
	encoder.SetIndent("", "  ")
	err = encoder.Encode(stats)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error encoding stats: %v\n", err)
		os.Exit(1)
	}
}
