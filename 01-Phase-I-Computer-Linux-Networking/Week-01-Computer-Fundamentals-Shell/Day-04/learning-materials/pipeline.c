#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
int pipefd[2];
if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
}
pid_t pid1 = fork(); //child process 1
if (pid1 == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
}
if(pid1 == 0){
    if(dup2(pipefd[1], STDOUT_FILENO) == -1){
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    execlp("ls", "ls", NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
}

pid_t pid2 = fork();

if(pid2 == -1){
    perror("fork");
    exit(EXIT_FAILURE);
}
if(pid2 == 0){
    if(dup2(pipefd[0], STDIN_FILENO) == -1){
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    execlp("grep", "grep", "txt", NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
}
close(pipefd[0]);
close(pipefd[1]);
waitpid(pid1, NULL, 0);
waitpid(pid2, NULL, 0);

return 0;

}