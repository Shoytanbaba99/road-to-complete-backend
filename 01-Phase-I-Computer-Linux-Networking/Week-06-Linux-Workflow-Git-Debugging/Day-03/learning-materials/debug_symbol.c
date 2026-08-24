#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void function_c(int *danger_ptr) {
    printf("[*] Inside function_c: Preparing to write to pointer...\n");
    // If danger_ptr is NULL, this will trigger a Hardware Page Fault / Segfault
    *danger_ptr = 999;
}

void function_b(int *ptr) {
    int local_b_var = 42;
    printf("[*] Inside function_b: local_b_var = %d\n", local_b_var);
    function_c(ptr);
}

void function_a(int *ptr) {
    int counter = 100;
    printf("[*] Inside function_a: Initial counter = %d\n", counter);
    
    // Increment counter to test Watchpoints
    counter += 50;
    printf("[*] Inside function_a: Mutated counter = %d\n", counter);

    function_b(ptr);
}

int main(int argc, char **argv) {
    printf("[+] Program started with PID: %d\n", getpid());
    
    int valid_memory = 10;
    int *target = &valid_memory;

    // If an argument is passed, deliberately corrupt the pointer to NULL
    if (argc > 1) {
        printf("[!] Sabotage flag detected: Setting target pointer to NULL\n");
        target = NULL;
    }

    function_a(target);

    printf("[+] Execution finished successfully. Valid memory = %d\n", valid_memory);
    return 0;
}