# Week 7 - Day 7 Capstone Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of the Week 7 Capstone CLI Task Tracker: CLI Argument Dispatching (`os.Args`), Interface Contracts (`TaskManager`), In-Memory State (`map[int]*Task`), and `sort.Slice` Sorting.

---

## 🌐 Capstone Architecture

```text
[ CLI INPUT (os.Args) ]
  ├── command: "add", "list", "get", "complete", "delete"
  └── args: task name, description, ID
        │
        ▼
[ TASK MANAGER INTERFACE (task.TaskManager) ]
  ├── AddTask(name, desc) (*Task, error)
  ├── List() []*Task
  ├── GetTask(id) (*Task, error)
  ├── CompleteTask(id) error
  └── DeleteTask(id) error
        │
        ▼
[ CONCRETE IMPLEMENTATION (task.InMemoryTaskManager) ]
  ├── Storage: map[int]*Task
  ├── Auto-Increment ID Counter: nextId
  └── Sorting: sort.Slice(result, func(i, j) { return result[i].Id < result[j].Id })
```

---

## 1. Key Engineering Takeaways from Week 7 Capstone

| Capstone Pattern | Go Implementation | Engineering Purpose |
|---|---|---|
| **CLI Argument Dispatching** | `os.Args[1]` with `switch` statement | Simple stdlib CLI command execution without third-party dependencies. |
| **Interface Decoupling** | `type TaskManager interface` | Separates command handling from storage logic; enables easy swapping for SQL storage later. |
| **Map Storage & ID Tracking** | `map[int]*Task` + `nextId` counter | Fast $O(1)$ key lookup by ID; pointer storage avoids copying struct memory. |
| **Deterministic Sorting** | `sort.Slice(result, func(i, j int) bool)` | Maps have non-deterministic iteration order; slice sorting guarantees ordered CLI list output. |
| **Custom Status Types** | `type TaskStatus string` | Type-safe enum pattern using constants (`Pending`, `Completed`). |
