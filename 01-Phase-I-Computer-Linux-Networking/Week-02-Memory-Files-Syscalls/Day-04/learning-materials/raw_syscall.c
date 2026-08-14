#include <unistd.h>
#include <sys/syscall.h> // Contains the integer IDs for system calls (e.g., SYS_write)
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main() {
    char message[] = "This bypassed printf. Going straight to the kernel.\n";
    
    // Instead of using printf() or even the POSIX write() wrapper, 
    // we explicitly load the syscall integer (SYS_write) and trigger the mode switch.
    // SYS_write requires 3 arguments: File Descriptor, Buffer Pointer, Length.
    long result = syscall(SYS_write, STDOUT_FILENO, message, strlen(message));

    if (result < 0) {
        printf("Syscall failed.\n");
        return 1;
    }

    printf("Kernel successfully wrote %ld bytes.\n\n", result);

    // ---------------------------------------------------------
    // DELIBERATE BREAKAGE SANDBOX
    // Uncomment one of the breakages below to observe kernel defenses.
    // ---------------------------------------------------------

    /* --- BREAKAGE 1: The Forged Syscall (Invalid ID) --- */
    // printf("Attempting Syscall ID 9999...\n");
    // long bad_id_result = syscall(9999, 0, 0, 0);
    // if (bad_id_result < 0) {
    //     perror("Kernel rejected Syscall ID 9999");
    // }

    /* --- BREAKAGE 2: The Malicious Pointer (Kernel Memory Assault) --- */
    // printf("Attempting to trick the kernel into reading invalid memory...\n");
    // // We will pass a NULL pointer to the write syscall.
    // char *malicious_ptr = NULL; 
    // long bad_ptr_result = syscall(SYS_write, STDOUT_FILENO, malicious_ptr, 10);
    
    // if (bad_ptr_result < 0) {
    //     perror("Kernel rejected the malicious pointer");
    // }

    return 0;
}