#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 20000

// Function for each thread
void* thread_function(void* arg) {
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    struct timespec start, end;

    // Record start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create 20,000 threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Join all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Record end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate time difference in seconds and nanoseconds
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Time taken to create %d threads: %f seconds\n", NUM_THREADS, time_taken);

    return 0;
}
