## 💡 Key Takeaways

Whenever a process is initialised, it gets its own virtual address space. This means that each process has its own memory space, which is isolated from other processes. we know of bss having uninitalised variables and data segment containing hte initialised varibles and code segment containing the executable code. The stack segment is used for function calls and local variables, while the heap segment is used for dynamic memory allocation.

The Stack segment is started form high memory and goes downards, while the heap segment starts from low memory and grows upwards. The stack is managed automatically by the operating system, while the heap requires manual management by the programmer.

the reason for heap segments existences is that what if we require lets say a variable or something from a function that has already returned, in that case the stack memory would be lost, but if we allocate it on the heap, it will still be accessible even after the function has returned.

And then there is malloc allocating **memory** on the heap, and calloc which allocates memory and removes the old data.
