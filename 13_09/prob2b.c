#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 1000
#define STACK_SIZE 1024 * 1024  // Stack size for each thread

// Function for the thread to execute
int thread_func(void* arg) {
    return 0;  // Exit immediately
}

int main() {
    void* stack;
    pid_t pids[NUM_THREADS];
    struct timespec start, end;

    // Record the start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_THREADS; i++) {
        // Allocate memory for the stack
        stack = malloc(STACK_SIZE);
        if (!stack) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        // Create a new thread using clone()
        pids[i] = clone(thread_func, stack + STACK_SIZE, CLONE_VM | SIGCHLD, NULL);
        if (pids[i] == -1) {
            perror("clone");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        waitpid(pids[i], NULL, 0);
    }

    // Record the end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate the time difference in seconds
    double time_taken = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Time taken to create %d threads: %f seconds\n", NUM_THREADS, time_taken);

    return 0;
}
