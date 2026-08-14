#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    printf("Step 1: This text is going to FD 1, which currently points to the terminal.\n");

    // Open a regular file on the disk. The OS will give us the lowest available FD (likely 3).
    // Flags: O_WRONLY (Write only), O_CREAT (Create if missing), O_TRUNC (Clear it if it exists).
    // 0644 are the file permissions (rw-r--r--).
    int target_fd = open("/tmp/hijacked_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (target_fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    printf("Step 2: Opened file. The OS gave us File Descriptor: %d\n", target_fd);

    // The Hijack: dup2(oldfd, newfd)
    // This tells the kernel: "Take whatever resource 'target_fd' is pointing to, 
    // and forcefully make FD 1 (stdout) point to that exact same resource."
    if (dup2(target_fd, STDOUT_FILENO) < 0) {
        perror("Failed to duplicate file descriptor");
        return 1;
    }

    // Now that FD 1 points to the file, we no longer need the original FD 3 pointer.
    close(target_fd);

    // Step 3: Proving the Hijack
    // printf() is hardcoded to write to FD 1. It has no idea we changed the plumbing.
    printf("Step 3: This text is printed using printf(), but it will NEVER appear on your screen.\n");
    printf("It has been secretly routed to /tmp/hijacked_output.txt.\n");

    // ---------------------------------------------------------
    // DELIBERATE BREAKAGE SANDBOX
    // Uncomment one of the breakages below to observe system failure.
    // ---------------------------------------------------------

    /* --- BREAKAGE 1: Resource Exhaustion (FD Leak) --- */
    // printf("Initiating Resource Exhaustion...\n");
    // int count = 0;
    // while (1) {
    //     int fd = open("/dev/null", O_RDONLY);
    //     if (fd < 0) {
    //         // We use fprintf to stderr (FD 2) because we broke stdout earlier!
    //         fprintf(stderr, "\nCRASH: The OS refused to give us more FDs!\n");
    //         fprintf(stderr, "Total FDs opened before failure: %d\n", count);
    //         perror("Kernel Reason");
    //         break;
    //     }
    //     count++;
    // }

    /* --- BREAKAGE 2: Writing to a Closed / Invalid FD --- */
    int bad_fd = 999; 
    // FD 999 doesn't exist in our table.
    ssize_t bytes_written = write(bad_fd, "Hello", 5);
    if (bytes_written < 0) {
        fprintf(stderr, "\nCRASH: Attempted to write to FD %d\n", bad_fd);
        perror("Kernel Reason");
    }

    return 0;
}