#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
    int fd_in = open("input.txt", O_RDONLY);
    if (fd_in < 0) {
        perror("Failed to open input.txt");
        return 1;
    }
    int fd_out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("Failed to open output.txt");
        close(fd_in);
        return 1;
    }
    int fd_err = open("error.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_err < 0) {
        perror("Failed to open error.txt");
        close(fd_in);
        close(fd_out);
        return 1;
    }
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("Failed to create pipe");
        close(fd_in);
        close(fd_out);
        close(fd_err);
        return 1;
    }
    pid_t pid1 = fork();
    if(pid1 == -1){
        perror("Failed to fork first child");
        close(fd_in);
        close(fd_out);
        close(fd_err);
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }
    if(pid1 == 0){
        // First child process
        if(dup2(fd_in,STDIN_FILENO) < 0 || dup2(pipefd[1],STDOUT_FILENO) < 0 || dup2(fd_err, STDERR_FILENO) < 0){
            perror("Failed to redirect file descriptors in first child");
            exit(1);
        };
        close(fd_in);
        close(fd_out);
        close(fd_err);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("grep","grep","ERROR",NULL);
        perror("Failed to exec grep");
        exit(1);
    }

    pid_t pid2 = fork();
    if(pid2 == -1){
        perror("Failed to fork second child");
        close(fd_in);
        close(fd_out);
        close(fd_err);
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }
    if(pid2 == 0){
        // Second child process
        if(dup2(fd_out,STDOUT_FILENO) < 0 || dup2(fd_err, STDERR_FILENO) < 0 || dup2(pipefd[0],STDIN_FILENO) < 0){
            perror("Failed to redirect file descriptors in second child");
            exit(1);
        };
        close(fd_in);
        close(fd_out);
        close(fd_err);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("sort","sort", "-r", NULL);
        perror("Failed to exec sort");
        exit(1);
    }
    close(fd_in);
    close(fd_out);
    close(fd_err);
    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);
    return 0;
}