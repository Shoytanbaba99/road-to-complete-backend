### Phase 1: The Generation Trap

**The Core Problem:**
In systems programming, programs model real-world entities (like network sockets, database connections, user sessions, or packets) that consist of multiple related data fields and operations acting on that data.

In low-level paradigms (like C):

1. Data layout is defined as contiguous memory chunks, but functions that manipulate that data live completely detached in global namespace scope (e.g., `user_set_name(&u, "Alice")` vs OOP-style `user.SetName("Alice")`).
2. When passing large chunks of structured state between functions, passing by value duplicates every byte across stack frames, consuming memory bandwidth and CPU cycles.
3. If you pass by reference (using memory addresses/pointers) to allow in-place mutations or save stack memory, you risk accessing uninitialized memory filled with random leftover garbage bits, or creating dangling pointers to stack frames that no longer exist once a function returns.
4. Many languages either introduce massive object-oriented overhead (hidden virtual method tables `vtables`, class inheritance hierarchies, constructor boilerplate) or leave memory entirely unmanaged and vulnerable to undefined behavior from uninitialized variables.

Go needed a way to group heterogenous data contiguously in memory, attach behavior directly to types without heavyweight class hierarchies or dynamic dispatch overhead, allow safe in-place mutations via references without pointer arithmetic hazards, and guarantee that memory is never left in an uninitialized, garbage-bit state.

**Your Task:**
If you were the systems engineer tasked with designing a language's data-modeling and memory-access mechanism from scratch:

1. How would you design a way to group multiple variables together and bind functions directly to that grouped data?
2. How would you handle passing data between functions efficiently (without copying massive memory blocks) while preventing uninitialized memory bugs, without building a giant object-oriented runtime engine?

What naive approach would you take, and where do you think it would break?

_I am waiting for your answer._
