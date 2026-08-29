# Week 7 - Day 5 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Implicit Interfaces, Struct Composition vs. Inheritance, Package Visibility Boundaries (Export Rules), and Dependency Inversion.

---

## 🌐 Implicit Interface & Package Boundary Architecture

```text
[ PACKAGE: metrics ]
  type Publisher interface {          <-- Exported Interface Contract
      Publish(name string, val float64) error
  }
  type consoleEmitter struct {}        <-- Unexported Private Implementation
  func NewConsoleEmitter() Publisher   <-- Exported Constructor returning Interface

[ PACKAGE: engine ]
  type Worker struct {                <-- Decoupled Consumer
      id        int
      Publisher metrics.Publisher     <-- Accepts ANY type satisfying Publisher
  }

[ PACKAGE: main ]
  publisher := metrics.NewConsoleEmitter() ──► Passed to ──► engine.NewWorker(1, publisher)
```

---

## 1. Core Go Interfaces & Composition Rules

| Architectural Concept | Go Implementation | Engineering Benefit |
|---|---|---|
| **Implicit Interfaces** | No `implements` keyword. Types satisfy interfaces by implementing methods. | Duck typing with compile-time type safety; zero coupling between producer and consumer. |
| **Package Visibility** | `UpperCamelCase` = Exported (Public)<br>`lowerCamelCase` = Unexported (Private) | Enforces clean API boundaries and protects internal package state. |
| **Struct Composition** | Embedded fields (`type Student struct { Address }`) | Prefers composition over OOP inheritance hierarchies; avoids deep class trees. |
| **Interface Encapsulation** | Variable of interface type hides concrete struct fields | Callers can only invoke interface methods, preventing tight coupling to internal data. |
