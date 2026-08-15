#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    if (argc < 3){
        fprintf(stderr, "Usage: %s <timeout_commands> <command> [args ...]\n", argv[0]);
        return 1;
    }
    double timeout = atof(argv[1]);
    if (timeout <= 0){
        fprintf(stderr, "Invalid timeout value: %s\n", argv[1]);
        return 1;
    }
    pid_t child_pid = fork();
    if (child_pid < 0){
        perror("fork failed");
        return 1;
    }
    if(child_pid == 0){
        execvp(argv[2], &argv[2]);
        perror("execvp failed");
        exit(1);
    }

    struct timespec start_time, current_time;
    if(clock_gettime(CLOCK_MONOTONIC, &start_time) != 0){
        perror("clock_gettime failed");
        kill(child_pid, SIGKILL);
        return 1;
    }
    int status;
    while(1){
        pid_t result = waitpid(child_pid, &status, WNOHANG);
        if(result == child_pid){
                if(WIFEXITED(status)){
                    printf("Child process exited with status %d\n", WEXITSTATUS(status));
                } else if(WIFSIGNALED(status)){
                    printf("Child process terminated by signal %d\n", WTERMSIG(status));
                }
                return EXIT_SUCCESS;
        }else if(result < 0){
            perror("waitpid failed");
            kill(child_pid, SIGKILL);
            return 1;
        }
        if(clock_gettime(CLOCK_MONOTONIC, &current_time) != 0){
            perror("clock_gettime failed");
            kill(child_pid, SIGKILL);
            return 1;
        }
        double elapsed_time = (current_time.tv_sec - start_time.tv_sec) + (current_time.tv_nsec - start_time.tv_nsec) / 1e9;
        if(elapsed_time >= timeout){
                printf("Timeout reached. Killing child process.\n");
                kill(child_pid, SIGKILL);
                waitpid(child_pid, &status, 0);
                return 1;
            }
            struct timespec sleep_time = {0, 10000000}; // Sleep for 10 milliseconds
            nanosleep(&sleep_time, NULL);
    }
    return EXIT_SUCCESS;
}