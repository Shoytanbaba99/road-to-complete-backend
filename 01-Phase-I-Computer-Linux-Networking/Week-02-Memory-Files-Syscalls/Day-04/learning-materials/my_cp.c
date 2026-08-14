#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char*argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }
    const char *source_file = argv[1];
    const char *destination_file = argv[2];

    int source_fd = open(source_file, O_RDONLY);
    if(source_fd < 0){
        perror("Failed to open source file");
        return 1;
    }
    int dest_fd = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dest_fd < 0){
        perror("Failed to open destination file");
        close(source_fd);
        return 1;
    }
    char buffer [BUFFER_SIZE];
    ssize_t bytes_read;
    while((bytes_read = read(source_fd,buffer,BUFFER_SIZE))> 0){
        ssize_t total_written = 0;
        while(total_written < bytes_read){
            ssize_t bytes_written = write(dest_fd, buffer + total_written, bytes_read - total_written);
            if(bytes_written < 0){
                perror("Failed to write to destination file");
                close(source_fd);
                close(dest_fd);
                return 1;
            }
            total_written += bytes_written;
        }
    }
    close(source_fd);
    close(dest_fd);
    return 0;

}