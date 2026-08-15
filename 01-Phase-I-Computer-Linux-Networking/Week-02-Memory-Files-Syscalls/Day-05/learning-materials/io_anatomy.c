#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    printf("=== PHASE 1: User-Space Buffering (The C Library) ===\n");
    // fopen uses FILE*, which is a buffered stream managed by user-space heap.
    FILE *stream = fopen("user_buffer.txt", "w");
    if (!stream) return 1;

    // We write to the stream. Because the buffer isn't full and there is no newline,
    // this data NEVER reaches the kernel. It sits in the program's RAM.
    fprintf(stream, "This is buffered data. It is trapped in user-space.");
    printf("Data passed to fprintf. Check the file in another terminal.\n");
    printf("Press Enter to forcefully crash the program...\n");
    getchar();

    // DELIBERATE BREAKAGE 1: Process Death
    // By calling abort(), we kill the process immediately. The C library 
    // never gets a chance to call fflush() or close(). 
    // If you uncomment the line below, the data is permanently lost.
    // abort(); 

    fclose(stream); // This gracefully flushes the buffer to the kernel.

    printf("\n=== PHASE 2: Kernel-Space Buffering (Page Cache) ===\n");
    // We use the raw POSIX system call. No user-space buffering exists here.
    int fd = open("kernel_buffer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    char payload[] = "This bypassed the C library. It is trapped in the Kernel Page Cache.\n";
    
    // The syscall pushes data across the boundary into the OS RAM.
    ssize_t bytes = write(fd, payload, strlen(payload));
    printf("Wrote %ld bytes via syscall.\n", bytes);
    
    // Even if we abort() here, the data survives process death, because the 
    // kernel owns it now. BUT if someone kicks the power cord out of the wall 
    // right now, the data is lost because the physical disk hasn't spun yet.
    
    // DELIBERATE BREAKAGE 2: The fsync Guarantee
    printf("Forcing physical disk write via fsync...\n");
    // fsync halts the CPU until the SSD/HDD controller physically confirms the write.
    if (fsync(fd) < 0) {
        perror("fsync failed");
    } else {
        printf("fsync complete. Data is physically indestructible now.\n");
    }

    close(fd);

    printf("\n=== PHASE 3: Reading Metadata (The Inode) ===\n");
    struct stat file_meta;
    if (stat("kernel_buffer.txt", &file_meta) == 0) {
        printf("Inode Number: %lu\n", file_meta.st_ino);
        printf("File Size: %ld bytes\n", file_meta.st_size);
        printf("Physical Disk Blocks allocated: %ld\n", file_meta.st_blocks);
        // Notice that st_blocks * 512 (standard block size) is often larger 
        // than st_size. The OS allocates disk space in chunks, not exact bytes.
    }

    return 0;
}