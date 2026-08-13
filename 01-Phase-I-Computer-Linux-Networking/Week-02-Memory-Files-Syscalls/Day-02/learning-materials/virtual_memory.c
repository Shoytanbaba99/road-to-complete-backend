#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // 1 Gigabyte of memory
    size_t gb = 1024 * 1024 * 1024; 
    
    printf("PID: %d\n", getpid());
    printf("Step 1: About to request 1GB of Virtual Memory.\n");
    printf("Check 'top' in another terminal. Press Enter to continue...\n");
    getchar();

    // The OS updates the Page Table, setting the Valid bits to 0 (Invalid).
    // It allocates ALMOST ZERO physical RAM.
    char *massive_array = (char *)malloc(gb);
    
    if (massive_array == NULL) {
        printf("Malloc failed!\n");
        return 1;
    }

    printf("Step 2: 1GB Virtual Memory allocated at Virtual Address: %p\n", (void*)massive_array);
    printf("Look at 'top' again. The 'VIRT' column increased by 1G, but 'RES' (Physical RAM) barely moved.\n");
    printf("Press Enter to begin forcing Page Faults...\n");
    getchar();

    // Step 3: Triggering Minor Page Faults
    // We are going to write 1 byte into every 4KB page.
    // This forces the MMU to throw a Page Fault on every loop iteration, 
    // forcing the OS to find a physical 4KB frame and map it.
    size_t page_size = 4096;
    for (size_t i = 0; i < gb; i += page_size) {
        massive_array[i] = 'A';
    }

    printf("Step 3: Finished triggering 262,144 Page Faults.\n");
    printf("Look at 'top' now. 'RES' has spiked to 1GB because we forced physical allocation.\n");
    
    printf("\n=== INITIATING DELIBERATE BREAKAGE ===\n");
    printf("Press Enter to attempt an illegal memory access...\n");
    getchar();

    // Deliberate Breakage: Bypassing the valid Virtual Address Space
    // We take our valid pointer, and jump 1 byte past the 1GB allocation.
    // Because this Virtual Page was never allocated in our Page Table, the OS
    // has no mapping for it.
    char *illegal_address = massive_array + gb + 1;
    
    printf("Attempting to write to unmapped Virtual Address: %p\n", (void*)illegal_address);
    
    // The MMU looks up the VPN, sees the Page Table has no entry, throws a Page Fault.
    // The OS kernel catches the fault, realizes we never called malloc() for this page, 
    // and brutally murders the process.
    *illegal_address = 'X'; 
    
    // We will never reach this line.
    printf("Successfully wrote to illegal address!\n");

    free(massive_array);
    return 0;
}