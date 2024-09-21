#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024)  


int child_func(void *arg) {
    printf("World (from clone)\n");
    return 0;
}

int main() {
    printf("=== Using fork() ===\n");
    pid_t pid_fork = fork();

    if (pid_fork == 0) {
        
        printf("World (from fork)\n");
        _exit(0);  
    } else if (pid_fork > 0) {
        
        printf("Hello (from fork)\n");
        wait(NULL);  
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    printf("\n=== Using fork() + exec() ===\n");
    pid_t pid_exec = fork();

    if (pid_exec == 0) {
        
        execl("/bin/echo", "echo", "World (from exec)", (char *)NULL);
        perror("execl");  
        _exit(1);
    } else if (pid_exec > 0) {
        
        printf("Hello (from exec)\n");
        wait(NULL);  
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    printf("\n=== Using clone() ===\n");
    char *stack = malloc(STACK_SIZE);  
    if (!stack) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *stack_top = stack + STACK_SIZE;  

    
    pid_t pid_clone = clone(child_func, stack_top, SIGCHLD, NULL);
    if (pid_clone == -1) {
        perror("clone");
        free(stack);
        exit(EXIT_FAILURE);
    }

    
    printf("Hello (from clone)\n");
    wait(NULL);  
    free(stack);  

    return 0;
}
