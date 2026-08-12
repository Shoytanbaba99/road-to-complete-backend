#include <stdio.h>
#include <stdlib.h>

int main() {
    // 1. Allocate exactly 4 bytes of memory in RAM
    int* data_pointer = (int*)malloc(sizeof(int));
    *data_pointer = 255; // 255 in hex is 0x000000FF

    // 2. Prove the memory abstraction by printing the RAM address
    printf("The data is living at RAM address: %p\n", (void*)data_pointer);
    printf("The data contains the value: %d\n", *data_pointer);

    // 3. Deliberately breaking the abstraction (Buffer Overread/Overwrite)
    printf("\n--- INITIATING DELIBERATE BREAKAGE ---\n");

    // We only asked the OS for 4 bytes. We are now going to tell the CPU
    // to jump 1,000,000 bytes past our allowed address space and read it.
    int* illegal_pointer = data_pointer + 1000000;

    printf("Attempting to read RAM address: %p\n", (void*)illegal_pointer);

    // The moment the CPU executes the instruction to fetch this address,
    // the hardware Memory Management Unit (MMU) will intercept it and panic.
    int secret_data = *illegal_pointer;

    // We will never reach this line.
    printf("Successfully stole data: %d\n", secret_data);

    free(data_pointer);
    return 0;
}
