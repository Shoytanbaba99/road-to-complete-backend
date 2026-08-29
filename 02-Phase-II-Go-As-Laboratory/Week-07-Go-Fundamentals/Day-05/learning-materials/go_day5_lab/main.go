package main

import (
	"day5lab/engine"
	"day5lab/metrics"
	"fmt"
)

func main() {
	fmt.Println("=== PIPELINE STARTING ===")
	fmt.Println()

	publisher := metrics.NewConsoleEmitter()
	worker := engine.NewWorker(1, publisher)

	err := worker.ProcessJob("data_processing", 2.5)
	if err != nil {
		fmt.Println("Error:", err)
	}
	err = worker.ProcessJob("image_upload", 1.2)
	if err != nil {
		fmt.Println("Error:", err)
	}
	err = worker.ProcessJob("email_send", 0.8)
	if err != nil {
		fmt.Println("Error:", err)
	}

	fmt.Println()
	fmt.Println("=== PIPELINE COMPLETE ===")
}
