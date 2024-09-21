#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 10

// Thread function for each thread
void* thread_function(void* arg) {
    int thread_id = *((int*)arg);
    printf("Thread %d started.\n", thread_id);

    // Sleep for demonstration purposes (simulating some work)
    sleep(1);

    if (thread_id % 2 == 0) {
        // Even-numbered threads: return the thread ID back to the parent using pthread_exit
        printf("Thread %d (even, joinable) exiting with value %d.\n", thread_id, thread_id);
        pthread_exit((void*)(intptr_t)thread_id);
    } else {
        // Odd-numbered threads: detach and exit
        printf("Thread %d (odd, detached) exiting.\n", thread_id);
        pthread_exit(NULL);
    }
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;  // Thread IDs start from 1

        // Initialize thread attributes
        pthread_attr_init(&attr);

        if (thread_ids[i] % 2 == 0) {
            // Even-numbered threads: create in joinable mode (default mode)
            printf("Creating joinable thread %d...\n", thread_ids[i]);
        } else {
            // Odd-numbered threads: set as detached
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            printf("Creating detached thread %d...\n", thread_ids[i]);
        }

        // Create the thread
        if (pthread_create(&threads[i], &attr, thread_function, (void*)&thread_ids[i]) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }

        // Destroy thread attribute object to free resources
        pthread_attr_destroy(&attr);
    }

    // Wait for joinable (even-numbered) threads to finish and retrieve their exit values
    for (int i = 0; i < NUM_THREADS; i++) {
        if (thread_ids[i] % 2 == 0) {
            void* ret_val;
            if (pthread_join(threads[i], &ret_val) != 0) {
                perror("Failed to join thread");
                exit(EXIT_FAILURE);
            }
            printf("Parent: Joined thread %d, it returned value %d.\n", thread_ids[i], (int)(intptr_t)ret_val);
        }
    }

    printf("Parent process completed.\n");
    return 0;
}
