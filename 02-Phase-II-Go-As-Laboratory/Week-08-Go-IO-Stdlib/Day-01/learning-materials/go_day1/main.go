package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type TaskStatus string

const (
	Pending   TaskStatus = "Pending"
	Completed TaskStatus = "Completed"
)

type Task struct {
	Id          string
	Name        string
	Description string
	status      TaskStatus
}

type TaskManager interface {
	AddTask(name string, description string) (*Task, error)
	List() []*Task
	GetTask(id string) (*Task, error)
	CompleteTask(id string) error
	DeleteTask(id string) error
	SaveTasksToFile(filepath string, tasks []*Task) error
	LoadTasksFromFile(filepath string) ([]*Task, error)
}

type InMemoryTaskManager struct {
	tasks  map[string]*Task
	nextId int
}

func NewInMemoryTaskManager() *InMemoryTaskManager {
	return &InMemoryTaskManager{
		tasks:  make(map[string]*Task),
		nextId: 1,
	}
}

func (tm *InMemoryTaskManager) AddTask(name string, description string) (*Task, error) {
	task := &Task{
		Id:          fmt.Sprintf("task_%d", tm.nextId),
		Name:        name,
		Description: description,
		status:      Pending,
	}
	tm.tasks[task.Id] = task
	tm.nextId++
	return task, nil
}
func (tm *InMemoryTaskManager) List() []*Task {
	var taskList []*Task
	for _, task := range tm.tasks {
		taskList = append(taskList, task)
	}
	return taskList
}
func (tm *InMemoryTaskManager) GetTask(id string) (*Task, error) {
	if task, exists := tm.tasks[id]; exists {
		return task, nil
	}
	return nil, fmt.Errorf("task not found")
}
func (tm *InMemoryTaskManager) CompleteTask(id string) error {
	if task, exists := tm.tasks[id]; exists {
		task.status = Completed
		return nil
	}
	return fmt.Errorf("task not found")
}
func (tm *InMemoryTaskManager) DeleteTask(id string) error {
	if _, exists := tm.tasks[id]; exists {
		delete(tm.tasks, id)
		return nil
	}
	return fmt.Errorf("task not found")
}
func (tm *InMemoryTaskManager) SaveTasksToFile(filepath string, tasks []*Task) error {
	file, err := os.Create(filepath)
	if err != nil {
		return err
	}
	defer file.Close()
	writer := bufio.NewWriter(file)
	for _, task := range tasks {
		_, err := writer.WriteString(fmt.Sprintf("%s|%s|%s|%s\n", task.Id, task.Name, task.Description, task.status))
		if err != nil {
			return err
		}
	}
	return writer.Flush()
}
func (tm *InMemoryTaskManager) LoadTasksFromFile(filepath string) ([]*Task, error) {
	file, err := os.Open(filepath)

	if err != nil {
		return nil, err
	}

	defer file.Close()

	tm.tasks = make(map[string]*Task)

	var tasks []*Task
	maxId := 0

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		parts := strings.Split(line, "|")

		if len(parts) != 4 {
			continue
		}

		task := &Task{
			Id:          parts[0],
			Name:        parts[1],
			Description: parts[2],
			status:      TaskStatus(parts[3]),
		}
		tm.tasks[task.Id] = task
		idNumber, err := strconv.Atoi(strings.TrimPrefix(task.Id, "task_"))

		if err == nil && idNumber > maxId {
			maxId = idNumber
		}

		tasks = append(tasks, task)
	}

	if err := scanner.Err(); err != nil {
		return nil, err
	}
	tm.nextId = maxId + 1

	return tasks, nil
}

func parseCommand(input string) []string {
	var parts []string
	var current strings.Builder
	inQuotes := false

	for _, r := range input {
		switch r {
		case ' ':
			if inQuotes {
				current.WriteRune(r)
			} else if current.Len() > 0 {
				parts = append(parts, current.String())
				current.Reset()
			}
		case '"':
			inQuotes = !inQuotes
		default:
			current.WriteRune(r)
		}
	}
	if current.Len() > 0 {
		parts = append(parts, current.String())
	}
	return parts
}

func main() {
	taskManager := NewInMemoryTaskManager()
	scanner := bufio.NewScanner(os.Stdin)

	fmt.Println("Welcome to the Task Manager!")
	fmt.Println("Commands: add, list, get, complete, delete, quit")

	for {
		fmt.Print("> ")

		if !scanner.Scan() {
			break
		}

		input := scanner.Text()
		parts := parseCommand(input)

		if len(parts) == 0 {
			continue
		}

		command := parts[0]

		switch command {

		case "add":
			if len(parts) < 3 {
				fmt.Println("Usage: add <name> <description>")
				continue
			}

			task, err := taskManager.AddTask(parts[1], parts[2])
			if err != nil {
				fmt.Println("Error:", err)
				continue
			}

			fmt.Printf("Task added: %+v\n", task)

		case "list":
			for _, task := range taskManager.List() {
				fmt.Printf("%+v\n", task)
			}

		case "get":
			if len(parts) < 2 {
				fmt.Println("Usage: get <id>")
				continue
			}

			task, err := taskManager.GetTask(parts[1])

			if err != nil {
				fmt.Println("Error:", err)
				continue
			}

			fmt.Printf("%+v\n", task)

		case "complete":
			if len(parts) < 2 {
				fmt.Println("Usage: complete <id>")
				continue
			}

			if err := taskManager.CompleteTask(parts[1]); err != nil {
				fmt.Println("Error:", err)
				continue
			}

			fmt.Println("Task completed")

		case "delete":
			if len(parts) < 2 {
				fmt.Println("Usage: delete <id>")
				continue
			}

			if err := taskManager.DeleteTask(parts[1]); err != nil {
				fmt.Println("Error:", err)
				continue
			}

			fmt.Println("Task deleted")
		case "save":
			if len(parts) < 2 {
				fmt.Println("Usage: save <filepath>")
				continue
			}

			tasks := taskManager.List()
			if err := taskManager.SaveTasksToFile(parts[1], tasks); err != nil {
				fmt.Println("Error saving tasks:", err)
				continue
			}

			fmt.Println("Tasks saved to file")

		case "load":
			if len(parts) < 2 {
				fmt.Println("Usage: load <filepath>")
				continue
			}

			tasks, err := taskManager.LoadTasksFromFile(parts[1])
			if err != nil {
				fmt.Println("Error loading tasks:", err)
				continue
			}

			fmt.Printf("Loaded %d tasks from file\n", len(tasks))

		case "quit", "exit":
			return

		default:
			fmt.Println("Unknown command")
		}
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error:", err)
	}
}
