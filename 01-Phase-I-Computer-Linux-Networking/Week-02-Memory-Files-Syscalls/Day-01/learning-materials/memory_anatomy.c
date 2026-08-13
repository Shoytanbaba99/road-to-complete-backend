#include <stdio.h>
#include <stdlib.h>

// A recursive function to deliberately cause a Stack Overflow
void infinite_recursion(int counter) {
    int large_array[1000]; // Consumes ~4KB of stack space per frame
    printf("Stack Frame %d allocated.\n", counter);
    infinite_recursion(counter + 1);
}

int main() {
    printf("=== MEMORY TOPOLOGY ===\n");
    
    // 1. Stack Allocation
    int stack_var = 42;
    printf("Address of stack_var: %p (High Memory)\n", (void*)&stack_var);

    // 2. Heap Allocation (Dynamic)
    int *heap_ptr = (int*)malloc(sizeof(int));
    *heap_ptr = 99;
    printf("Address of heap_ptr's payload: %p (Low Memory)\n", (void*)heap_ptr);
    
    // Proving pointers hold absolute addresses
    printf("Value inside stack_var: %d\n", stack_var);
    printf("Value inside heap_ptr payload: %d\n\n", *heap_ptr);

    // 3. Deliberate Breakage Sandbox
    printf("=== INITIATING DELIBERATE BREAKAGE ===\n");
    printf("Uncomment one of the breakage lines in the code to observe failure.\n");

    /* --- BREAKAGE 1: The Memory Leak --- */
     heap_ptr = NULL; 
    // We overwrote the only variable holding the address to our heap block.
    // The 4 bytes are permanently lost in RAM until the process dies.

    /* --- BREAKAGE 2: Use-After-Free (Dangling Pointer) --- */
     //free(heap_ptr);
     //printf("Data after free: %d\n", *heap_ptr); 
    // We gave the memory back, but the pointer still holds the address.
    // Reading it is undefined behavior. The OS might crash, or you might read garbage.

    /* --- BREAKAGE 3: Null Pointer Dereference --- */
    // int *null_ptr = NULL;
    // *null_ptr = 5; 
    // Hardware MMU strictly forbids writing to address 0x0. Instant Segmentation Fault.

    /* --- BREAKAGE 4: Stack Overflow --- */
    // infinite_recursion(1);
    // Recursively consumes 4KB chunks until the 8MB OS limit is breached. Instant crash.

    // Proper Cleanup
    free(heap_ptr);
    return 0;
}