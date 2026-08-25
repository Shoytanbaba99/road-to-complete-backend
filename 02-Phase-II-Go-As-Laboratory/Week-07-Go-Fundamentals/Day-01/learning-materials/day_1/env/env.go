package env

import (
	"os"
)

func GetMode() string {
	mode := os.Getenv("OP_MODE")
	if mode == "" {
		return "STANDARD"
	}
	return mode
}
