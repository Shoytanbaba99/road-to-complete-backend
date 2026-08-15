#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/syscall.h>

int main() {
    printf("PID: %d\n", getpid());
    
    // 1. Memory (The Heap)
    // We request memory. We know this triggers a brk() or mmap() syscall under the hood.
    char *buffer = (char *)malloc(1024);
    if (!buffer) return 1;

    // 2. Time (Monotonic Clock)
    // We measure absolute execution time, immune to NTP shifts.
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // 3. Files & Syscalls (The Bottleneck)
    // We bypass the C library and talk directly to the VFS.
    int fd = open("test_payload.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    // 4. The Payload
    snprintf(buffer, 1024, "This is raw data crossing the user/kernel boundary.\n");
    
    // We trigger the mode switch via SYS_write
    long bytes_written = syscall(SYS_write, fd, buffer, 53);
    
    // 5. Deliberate Time Delay
    // We force the OS to context switch us off the CPU for 1 second.
    sleep(1);

    // 6. Cleanup & Time Calc
    close(fd);
    free(buffer);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1e9);
    
    printf("Successfully wrote %ld bytes. Total monotonic time: %f seconds.\n", bytes_written, elapsed);

    // ---------------------------------------------------------
    // DELIBERATE BREAKAGE SANDBOX
    // ---------------------------------------------------------

    /* --- BREAKAGE 1: FD Exhaustion via strace --- */
    // while(1) { open("/dev/null", O_RDONLY); }

    /* --- BREAKAGE 2: The Malicious Read --- */
    // syscall(SYS_read, 999, NULL, 1024);

    return 0;
}