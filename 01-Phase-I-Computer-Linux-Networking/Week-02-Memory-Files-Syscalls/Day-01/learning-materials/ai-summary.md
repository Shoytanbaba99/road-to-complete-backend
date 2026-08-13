# Week 2 - Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Stack vs. Heap memory segments, Stack frame call mechanics, dynamic heap allocation (`malloc`/`calloc`/`realloc`/`free`), pointer dereferencing, and Heap Memory Leak prevention.

---

## 🌐 Virtual Address Space: Stack vs. Heap Memory Layout

```text
[ VIRTUAL ADDRESS SPACE (High Addresses 0x7FFFFFFFFFFF) ]
├── STACK SEGMENT  (Grows DOWNWARD ──► Stack Pointer SP decrements)
│   ├── main() Stack Frame (Local variables, return address)
│   └── func() Stack Frame (Auto-allocated, destroyed on function return)
│
│                        │ (Unallocated Memory Gap)
│                        ▼
│                        ▲
│                        │ (Heap Expansion via brk/sbrk/mmap syscalls)
├── HEAP SEGMENT   (Grows UPWARD   ──► Program Break Pointer increments)
│   ├── Node 1 [ malloc(sizeof(Node)) ]
│   └── Node 2 [ malloc(sizeof(Node)) ]  (Persists across function returns until free())
├── BSS / DATA SEGMENTS (Initialized & Uninitialized Globals)
└── TEXT SEGMENT   (Read-Only Executable Machine Code)
[ LOW ADDRESSES 0x00000000 ]
```

---

## 1. Stack Memory Segment (Automatic Allocation)
- **Growth Direction:** High memory addresses $\rightarrow$ Low memory addresses (grows **downward**).
- **Lifetime:** Managed automatically by CPU instruction pointers and stack frames. When a function returns, its stack frame is popped off immediately, invalidating local variables.
- **Speed:** Ultra-fast $O(1)$ allocation (adjusts the CPU Stack Pointer `RSP` register).
- **Limitation:** Fixed stack size per thread (typically 8 MB on Linux). Exceeding this limit causes a **Stack Overflow**.

---

## 2. Heap Memory Segment (Dynamic Allocation)
- **Growth Direction:** Low memory addresses $\rightarrow$ High memory addresses (grows **upward**).
- **Lifetime:** Managed manually by the programmer via `malloc()` / `free()`. Memory persists across function calls until explicitly released.
- **Syscall Mechanics:** The kernel expands the heap using `brk()` / `sbrk()` or allocates virtual memory pages via `mmap()`.
- **Memory Functions:**
  - `malloc(size)`: Allocates `size` bytes of uninitialized heap memory.
  - `calloc(num, size)`: Allocates zero-initialized heap memory.
  - `realloc(ptr, new_size)`: Resizes existing heap allocation.
  - `free(ptr)`: Releases heap memory back to the allocator.

---

## 3. Dynamic Linked List Capstone (`dynamic_linked_list.c`)
- Demonstrates heap allocation for persistence: nodes created inside loops persist in heap memory across iterations.
- **Leak Prevention:** Traverses the linked list, copying `current->next` to a temporary pointer before invoking `free(temp)` to safely release all allocated heap nodes without dangling pointers.
