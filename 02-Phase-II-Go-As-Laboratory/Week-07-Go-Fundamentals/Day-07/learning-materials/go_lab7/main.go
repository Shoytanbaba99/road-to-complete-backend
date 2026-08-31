package main

import (
	"fmt"
	"os"
	"strconv"
	"tasktracker/task"
)

func main() {
	defer func() {
		fmt.Println("Exiting the program...")
	}()

	if len(os.Args) < 2 {
		fmt.Println("Please provide a command: add, list, get, complete, delete")
		os.Exit(1)
	}
	command := os.Args[1]
	taskManager := task.NewInMemoryTaskManager()

	switch command {
	case "add":
		if len(os.Args) < 4 {
			fmt.Println("Please provide task name and description")
			os.Exit(1)
		}
		name := os.Args[2]
		description := os.Args[3]
		task, err := taskManager.AddTask(name, description)
		if err != nil {
			fmt.Println("Error adding task:", err)
			os.Exit(1)
		}
		fmt.Printf("Task added: %+v\n", task)
	case "list":
		tasks := taskManager.List()
		for _, task := range tasks {
			fmt.Printf("Task: %+v\n", task)
		}
	case "get":
		if len(os.Args) < 3 {
			fmt.Println("Please provide task ID")
			os.Exit(1)
		}
		idStr := os.Args[2]
		id, err := strconv.Atoi(idStr)
		if err != nil {
			fmt.Println("Error: task ID must be a number")
			os.Exit(1)
		}
		task, err := taskManager.GetTask(id)
		if err != nil {
			fmt.Println("Error getting task:", err)
			os.Exit(1)
		}
		fmt.Printf("Task: %+v\n", task)
	case "complete":
		if len(os.Args) < 3 {
			fmt.Println("Please provide task ID")
			os.Exit(1)
		}
		idStr := os.Args[2]

		id, err := strconv.Atoi(idStr)
		if err != nil {
			fmt.Println("Error: task ID must be a number")
			os.Exit(1)
		}
		err = taskManager.CompleteTask(id)
		if err != nil {
			fmt.Println("Error completing task:", err)
			os.Exit(1)
		}
		fmt.Println("Task completed successfully")
	case "delete":
		if len(os.Args) < 3 {
			fmt.Println("Please provide task ID")
			os.Exit(1)
		}
		idStr := os.Args[2]
		id, err := strconv.Atoi(idStr)
		if err != nil {
			fmt.Println("Error: task ID must be a number")
			os.Exit(1)
		}
		err = taskManager.DeleteTask(id)
		if err != nil {
			fmt.Println("Error deleting task:", err)
			os.Exit(1)
		}
		fmt.Println("Task deleted successfully")
	default:
		fmt.Println("Unknown command:", command)
		os.Exit(1)
	}
}
