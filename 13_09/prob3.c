#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define STACK_SIZE (1024 * 1024)  // Stack size for each thread
#define MAX_THREADS 16            // Maximum number of threads (generalized)

int prime_count = 0;  // Shared counter for the number of primes
pthread_mutex_t lock; // Mutex lock to avoid race conditions on prime_count

// Function to check if a number is prime
int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Function for each thread to execute (find primes in a given range)
int prime_finder(void *arg) {
    int *range = (int *)arg;
    int start = range[0];
    int end = range[1];
    
    int local_count = 0;  // Local count to reduce the need for synchronization
    
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            local_count++;
        }
    }
    
    // Lock the shared variable and update the global count
    pthread_mutex_lock(&lock);
    prime_count += local_count;
    pthread_mutex_unlock(&lock);
    
    free(range);  // Free dynamically allocated memory for range
    return 0;
}

int main() {
    int N, M;
    printf("Enter the range limit N: ");
    scanf("%d", &N);
    printf("Enter the number of threads (M): ");
    scanf("%d", &M);
    
    if (M > MAX_THREADS) {
        printf("Limiting to %d threads for this experiment.\n", MAX_THREADS);
        M = MAX_THREADS;
    }

    pthread_mutex_init(&lock, NULL);  // Initialize mutex

    // Allocate stack for each thread
    char *stacks[MAX_THREADS];
    for (int i = 0; i < M; i++) {
        stacks[i] = malloc(STACK_SIZE);
        if (!stacks[i]) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }

    // Calculate range for each thread
    int step = N / M;
    for (int i = 0; i < M; i++) {
        int *range = malloc(2 * sizeof(int));
        if (!range) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        range[0] = i * step + 1;  // Start of the range
        range[1] = (i == M - 1) ? N : (i + 1) * step;  // End of the range
        
        if (clone(prime_finder, stacks[i] + STACK_SIZE, SIGCHLD | CLONE_VM, range) == -1) {
            perror("clone");
            free(range);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < M; i++) {
        wait(NULL);
    }

    // Output the result
    printf("Total prime numbers in range [1, %d]: %d\n", N, prime_count);

    // Free resources
    for (int i = 0; i < M; i++) {
        free(stacks[i]);
    }

    pthread_mutex_destroy(&lock);  // Destroy mutex
    return 0;
}
