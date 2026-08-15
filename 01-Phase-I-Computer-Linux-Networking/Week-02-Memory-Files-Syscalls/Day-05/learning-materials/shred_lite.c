#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define CHUNK_SIZE 65536

int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_to_shred>\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_WRONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("Failed to get file status");
        close(fd);
        return 1;
    }
    if(!S_ISREG(st.st_mode)) {
        fprintf(stderr, "Error: %s is not a regular file\n", argv[1]);
        close(fd);
        return 1;
    }
    off_t file_size = st.st_size;
    printf("[*] Shredding file: %s (size: %ld bytes)\n", argv[1], file_size);
    printf("[*] Inode: %lu\n", st.st_ino);
    printf("[*] Targeting %ld bytes for overwriting...\n", file_size);

    char buffer[CHUNK_SIZE];
    memset(buffer, 0xFF, sizeof(buffer));

off_t bytes_remaining = file_size;

while (bytes_remaining > 0) {
    size_t to_write =
        (bytes_remaining > CHUNK_SIZE)
        ? CHUNK_SIZE
        : (size_t)bytes_remaining;

    ssize_t written = write(fd, buffer, to_write);

    if (written < 0) {
        perror("write");
        close(fd);
        return EXIT_FAILURE;
    }

    if (written == 0) {
        fprintf(stderr, "write returned 0\n");
        close(fd);
        return EXIT_FAILURE;
    }

    bytes_remaining -= written;
}

if (fsync(fd) < 0) {
    perror("fsync");
    close(fd);
    return EXIT_FAILURE;
}

if (close(fd) < 0) {
    perror("close");
    return EXIT_FAILURE;
}

if (unlink(argv[1]) < 0) {
    perror("unlink");
    return EXIT_FAILURE;
}


}
