package main

import (
	"bufio"
	"context"
	"encoding/json"
	"errors"
	"flag"
	"fmt"
	"os"
	"strings"
	"time"
)

type TaskStatus string

const (
	Pending   TaskStatus = "Pending"
	Completed TaskStatus = "Completed"
)

type Task struct {
	Id          string     `json:"id"`
	Name        string     `json:"name"`
	Description string     `json:"description"`
	Status      TaskStatus `json:"Status"`
}

type store interface {
	AddTask(name string, description string) (*Task, error)
	List() []*Task
	GetTask(id string) (*Task, error)
	CompleteTask(id string) error
	DeleteTask(id string) error
}

type InMemorystore struct {
	tasks  map[string]*Task
	nextId int
}
type PersistentStore struct {
	*InMemorystore
	filePath string
}
type AppConfig struct {
	StoragePath string `json:"storage_path"`
	Verbose     bool   `json:"verbose"`
}

func NewPersistentStore(filePath string) (*PersistentStore, error) {
	return &PersistentStore{
		InMemorystore: NewInMemorystore(),
		filePath:      filePath,
	}, nil
}

func (s *PersistentStore) ExportReport(ctx context.Context, destinationPath string) error {
	file, err := os.Create(destinationPath)
	if err != nil {
		return fmt.Errorf("failed to create report file: %w", err)
	}
	defer file.Close()

	tasks := s.List()
	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "  ")
	for _, task := range tasks {
		if err := encoder.Encode(task); err != nil {
			return fmt.Errorf("failed to encode task: %w", err)
		}
		if err := ctx.Err(); err != nil {
			return fmt.Errorf("operation canceled: %w", err)
		}
		time.Sleep(100 * time.Millisecond)
	}
	return nil
}

func NewInMemorystore() *InMemorystore {
	return &InMemorystore{
		tasks:  make(map[string]*Task),
		nextId: 1,
	}
}

func (tm *InMemorystore) AddTask(name string, description string) (*Task, error) {
	task := &Task{
		Id:          fmt.Sprintf("task_%d", tm.nextId),
		Name:        name,
		Description: description,
		Status:      Pending,
	}
	tm.tasks[task.Id] = task
	tm.nextId++
	return task, nil
}
func (tm *InMemorystore) List() []*Task {
	var taskList []*Task
	for _, task := range tm.tasks {
		taskList = append(taskList, task)
	}
	return taskList
}
func (tm *InMemorystore) GetTask(id string) (*Task, error) {
	if task, exists := tm.tasks[id]; exists {
		return task, nil
	}
	return nil, fmt.Errorf("task not found")
}
func (tm *InMemorystore) CompleteTask(id string) error {
	if task, exists := tm.tasks[id]; exists {
		task.Status = Completed
		return nil
	}
	return fmt.Errorf("task not found")
}
func (tm *InMemorystore) DeleteTask(id string) error {
	if _, exists := tm.tasks[id]; exists {
		delete(tm.tasks, id)
		return nil
	}
	return fmt.Errorf("task not found")
}

func (t *Task) Validate() error {
	if t.Name == "" {
		return fmt.Errorf("task name cannot be empty")
	}
	if t.Description == "" {
		return fmt.Errorf("task description cannot be empty")
	}
	if t.Status != Pending && t.Status != Completed {
		return fmt.Errorf("task status must be either 'Pending' or 'Completed'")
	}
	if t.Id == "" {
		return fmt.Errorf("task id cannot be empty")
	}
	return nil
}

func (s *PersistentStore) Load() error {
	file, err := os.Open(s.filePath)
	if os.IsNotExist(err) {
		return nil // No tasks to load, not an error
	}
	if err != nil {
		return fmt.Errorf("failed to open task file: %w", err)
	}
	defer file.Close()

	var tasks []*Task
	decoder := json.NewDecoder(file)
	if err := decoder.Decode(&tasks); err != nil {
		return fmt.Errorf("failed to decode tasks: %w", err)
	}
	for _, task := range tasks {
		if err := task.Validate(); err != nil {
			return fmt.Errorf("invalid task data: %w", err)
		}
		s.tasks[task.Id] = task
		if task.Id == "" {
			continue
		}
		var num int
		fmt.Sscanf(task.Id, "task_%d", &num)
		if num >= s.nextId {
			s.nextId = num + 1
		}
	}
	return nil
}

func (s *PersistentStore) Save() error {
	tasks := s.List()
	file, err := os.Create(s.filePath)
	if err != nil {
		return fmt.Errorf("failed to create task file: %w", err)
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "  ")
	if err := encoder.Encode(tasks); err != nil {
		return fmt.Errorf("failed to encode tasks: %w", err)
	}
	return nil
}

func DefaultConfig() AppConfig {
	return AppConfig{
		StoragePath: ".tasks.json",
		Verbose:     false,
	}
}

func LoadConfig() (AppConfig, error) {
	cfg := DefaultConfig()
	if envPath := os.Getenv("TASK_STORAGE_PATH"); envPath != "" {
		cfg.StoragePath = envPath
	}
	fs := flag.NewFlagSet(os.Args[0], flag.ContinueOnError)
	fs.StringVar(&cfg.StoragePath, "storage", cfg.StoragePath, "Path to the storage file")
	fs.BoolVar(&cfg.Verbose, "verbose", cfg.Verbose, "Enable verbose output")

	fs.Parse(os.Args[1:])
	return cfg, nil
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
	cfg, err := LoadConfig()
	if err != nil {
		fmt.Println("Error loading config:", err)
		return
	}

	store, err := NewPersistentStore(cfg.StoragePath)
	if err != nil {
		fmt.Println("Error creating persistent store:", err)
		return
	}
	if err := store.Load(); err != nil {
		fmt.Println("Error loading tasks:", err)
		return
	}

	if cfg.Verbose {
		fmt.Println("Verbose mode enabled")
		fmt.Printf("[Debug] Loaded tasks from %s with %d tasks\n", cfg.StoragePath, len(store.List()))
	}

	defer func() {
		if err := store.Save(); err != nil {
			fmt.Println("Error saving tasks:", err)
		}
	}()
	scanner := bufio.NewScanner(os.Stdin)

	fmt.Println("Welcome to the Task Manager!")
	fmt.Println("Commands: add, list, get, complete, delete, quit")

	for {
		fmt.Print("> ")

		if !scanner.Scan() {
			break
		}

		parts := parseCommand(scanner.Text())

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

			task, err := store.AddTask(parts[1], parts[2])
			if err != nil {
				fmt.Println("Error:", err)
				continue
			}

			fmt.Printf("Task added: %+v\n", task)

		case "list":
			for _, task := range store.List() {
				fmt.Printf("%+v\n", task)
			}

		case "get":
			if len(parts) < 2 {
				fmt.Println("Usage: get <id>")
				continue
			}

			task, err := store.GetTask(parts[1])

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

			if err := store.CompleteTask(parts[1]); err != nil {
				fmt.Println("Error:", err)
				continue
			}

			fmt.Println("Task completed")

		case "delete":
			if len(parts) < 2 {
				fmt.Println("Usage: delete <id>")
				continue
			}

			if err := store.DeleteTask(parts[1]); err != nil {
				fmt.Println("Error:", err)
				continue
			}

			fmt.Println("Task deleted")
		case "export":
			exportFlag := flag.NewFlagSet("export", flag.ExitOnError)
			timeoutStr := exportFlag.String("timeout", "5s", "Timeout duration (e.g., 5s, 1m)")
			exportFlag.Parse(parts[1:])
			args := exportFlag.Args()

			if len(args) < 1 {
				fmt.Println("Usage: export <destination_path> [--timeout=<duration>]")
				continue
			}

			destinationPath := args[0]
			timeout, err := time.ParseDuration(*timeoutStr)
			if err != nil {
				fmt.Println("Invalid timeout duration:", err)
				continue
			}
			ctx, cancel := context.WithTimeout(context.Background(), timeout)
			defer cancel()

			err = store.ExportReport(ctx, destinationPath)
			if err != nil {
				if errors.Is(err, context.DeadlineExceeded) {
					fmt.Println("Export operation timed out")
				} else {
					fmt.Println("Error exporting report:", err)
				}
				continue
			}
			fmt.Printf("Export completed successfully to %s\n", destinationPath)

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
