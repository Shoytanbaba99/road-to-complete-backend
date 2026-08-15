#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>


int main(void){
    pid_t child_pid = fork();
    if (child_pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(child_pid == 0){
        if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
            perror("ptrace");
            exit(EXIT_FAILURE);
        }

        execl("/bin/echo", "echo", "Hello, World!", NULL);

        execl("/usr/bin/echo", "echo", "Hello, World!", NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    int status;
    struct user_regs_struct regs;
    int is_entry = 1;

    waitpid(child_pid, &status, 0);

    printf("====================================================\n");
    printf("         MINI_STRACE: SYSTEM CALL INTERCEPTOR       \n");
    printf("====================================================\n");
    while(1){
        if(ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) < 0){
            perror("ptrace SYSCALL failed");
            break;
        }
        if(waitpid(child_pid, &status, 0) < 0){
            perror("waitpid failed");
            break;
        }
        if(WIFEXITED(status)){
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
            break;
        }
        if(WIFSIGNALED(status)){
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
            break;
        }

        if(ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) < 0){
            perror("ptrace GETREGS failed");
            break;
        }

        if (is_entry) {
            printf("[SYSCALL ENTRY] ID: %-4llu ", regs.orig_rax);
            if (regs.orig_rax == 1) {
                printf("--> sys_write (Pushing data to FD %llu)", regs.rdi);
            } else if (regs.orig_rax == 59) {
                printf("--> sys_execve (Loading binary)");
            } else if (regs.orig_rax == 231) {
                printf("--> sys_exit_group (Terminating process)");
            }
            printf("\n");
            is_entry = 0;
        } else {
            is_entry = 1;
        }
    }
    printf("===================================\n");
    return 0;


}