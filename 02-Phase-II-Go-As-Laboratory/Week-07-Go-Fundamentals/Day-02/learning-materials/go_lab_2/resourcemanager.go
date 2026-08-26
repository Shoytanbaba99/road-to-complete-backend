package main

import (
	"errors"
	"fmt"
)

type Resource struct {
	ID     int
	IsOpen bool
}

func AcquireResource(id int) (*Resource, error) {
	if id <= 0 {
		return nil, errors.New("invalid resource ID")
	}
	fmt.Printf("[RESOURCE %d] Allocated / Opened\n", id)
	return &Resource{ID: id, IsOpen: true}, nil
}

func (r *Resource) Close() {
	if r.IsOpen {
		r.IsOpen = false
		fmt.Printf("[RESOURCE %d] Cleaned up / Closed\n", r.ID)
	}
}

func ProcessWorkflow(id int, triggerError bool) (string, error) {
	res, err := AcquireResource(id)
	if err != nil {
		return "", err
	}
	// INVARIANT: Resource MUST be closed regardless of where we exit
	defer res.Close()

	if triggerError {
		return "", errors.New("simulated critical workflow failure")
	}

	return fmt.Sprintf("Success on resource %d", res.ID), nil
}

func runExample() {
	fmt.Println("--- Running Success Path ---")
	msg, err := ProcessWorkflow(101, false)
	fmt.Printf("Result: %s, Error: %v\n\n", msg, err)

	fmt.Println("--- Running Error Path ---")
	msg, err = ProcessWorkflow(202, true)
	fmt.Printf("Result: %s, Error: %v\n", msg, err)
}
