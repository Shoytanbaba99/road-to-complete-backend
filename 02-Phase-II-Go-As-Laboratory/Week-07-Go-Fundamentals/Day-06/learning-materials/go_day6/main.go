package main

import (
	"errors"
	"fmt"
)

var ErrWrongPassword = errors.New("wrong password")

type RateLimitError struct {
	WaitSeconds int
}

func (e *RateLimitError) Error() string {
	return "rate limit exceeded, wait for " + string(e.WaitSeconds) + " seconds"
}

func login(attempts int, password string) error {
	if attempts > 3 {
		return &RateLimitError{WaitSeconds: 60}
	}
	if password != "secret" {
		return ErrWrongPassword
	}
	return nil
}

func handleLogin(attempts int, password string) error {
	err := login(attempts, password)
	if err != nil {
		return fmt.Errorf("login failed: %w", err)
	}
	return nil
}

func main() {
	err := handleLogin(1, "wrong")
	if errors.Is(err, ErrWrongPassword) {
		fmt.Println("Please Try Again")
	} else {
		fmt.Println(err)
	}
	err2 := handleLogin(5, "secret")
	var rateLimitErr *RateLimitError
	if errors.As(err2, &rateLimitErr) {
		fmt.Println("Rate limit exceeded, please wait")
	}
}
