#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// This uninitialized global variable goes to the BSS segment
int global_uninitialized; 
// This initialized global variable goes to the Data segment
int global_initialized = 42;

int main() {
int stack_var = 10;

int *heap_var = (int*) malloc(sizeof(int));
*heap_var = 20;
printf(" --- MEMORY ADDRESS SPACE ---\n");
printf("Code (Text) Segment (Function address): %p\n", (void*)main);
printf("Data Segment (Variable address): %p\n", (void*)&global_initialized);
printf("BSS Segment (Variable address): %p\n", (void*)&global_uninitialized);
printf("Heap Segment (Variable address): %p\n", (void*)heap_var);
printf("Stack Segment (Variable address): %p\n", (void*)&stack_var);

printf("\n --- PROCESS INFORMATION ---\n");
printf("I am the Original Parent Process. My PID is: %d, My PPID is: %d\n", getpid(), getppid());

pid_t pid = fork();
if (pid < 0) {
    perror("fork failed");
    exit(1);
} else if(pid ==0){
    printf("I am the Child Process. My PID is: %d, My PPID is: %d\n", getpid(), getppid());
    printf("[Child] I am exiting gracefully now. \n");
    exit(0);
}else{
    // Parent Process Block
        // Normally, the parent MUST call wait() to reap the child's exit status.
        // wait(NULL);
        
        printf("\n[PARENT] I spawned a child with PID: %d.\n", pid);
        printf("[PARENT] DELIBERATE BREAKAGE: I am going to sleep for 60 seconds without calling wait().\n");
        printf("[PARENT] Open another terminal quickly and run: ps aux | grep %d\n", pid);
        
        // The parent sleeps, completely ignoring the dead child process.
        sleep(60); 
        printf("[PARENT] Waking up and exiting. The OS will clean up the mess.\n");
}






}