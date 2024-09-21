#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_PROCESSES 5000

int main() {
    struct timespec start, end;
    
    // Record the start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            _exit(0);  // Exit immediately
        } else if (pid > 0) {
            // Parent process waits for the child to finish
            wait(NULL);
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Record the end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate the time difference in seconds
    double time_taken = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Time taken to create %d child processes: %f seconds\n", NUM_PROCESSES, time_taken);
    return 0;
}
