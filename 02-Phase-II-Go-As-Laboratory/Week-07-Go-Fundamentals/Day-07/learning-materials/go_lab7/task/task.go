package task

import (
	"errors"
	"fmt"
	"sort"
)

type TaskStatus string

const (
	Pending   TaskStatus = "Pending"
	Completed TaskStatus = "Completed"
)

type Task struct {
	Id          int
	Name        string
	Description string
	Status      TaskStatus
}

var (
	ErrTaskNotFound         = errors.New("task not found")
	ErrEmptyTaskName        = errors.New("task name cannot be empty")
	ErrEmptyTaskDescription = errors.New("task description cannot be empty")
)

type TaskManager interface {
	AddTask(name string, description string) (*Task, error)
	List() []*Task
	GetTask(id int) (*Task, error)
	CompleteTask(id int) error
	DeleteTask(id int) error
}

type InMemoryTaskManager struct {
	tasks  map[int]*Task
	nextId int
}

func NewInMemoryTaskManager() *InMemoryTaskManager {
	return &InMemoryTaskManager{
		tasks:  make(map[int]*Task),
		nextId: 1,
	}
}

func (s *InMemoryTaskManager) AddTask(name string, description string) (*Task, error) {
	if name == "" {
		return nil, fmt.Errorf("%s", ErrEmptyTaskName)
	}
	if description == "" {
		return nil, fmt.Errorf("%s", ErrEmptyTaskDescription)
	}
	task := &Task{
		Id:          s.nextId,
		Name:        name,
		Description: description,
		Status:      Pending,
	}
	s.tasks[task.Id] = task
	s.nextId++
	return task, nil
}

func (s *InMemoryTaskManager) List() []*Task {
	result := make([]*Task, 0, len(s.tasks))
	for _, task := range s.tasks {
		result = append(result, task)
	}
	sort.Slice(result, func(i, j int) bool {
		return result[i].Id < result[j].Id
	})
	return result
}
func (s *InMemoryTaskManager) GetTask(id int) (*Task, error) {
	if task, ok := s.tasks[id]; ok {
		return task, nil
	}
	return nil, fmt.Errorf("%s", ErrTaskNotFound)
}
func (s *InMemoryTaskManager) CompleteTask(id int) error {
	if task, ok := s.tasks[id]; ok {
		task.Status = Completed
		return nil
	}
	return fmt.Errorf("%s", ErrTaskNotFound)
}

func (s *InMemoryTaskManager) DeleteTask(id int) error {
	if _, ok := s.tasks[id]; !ok {
		return fmt.Errorf("%s", ErrTaskNotFound)
	}
	delete(s.tasks, id)
	return nil
}
