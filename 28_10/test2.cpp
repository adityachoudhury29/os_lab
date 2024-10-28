#include <iostream>
#include <thread>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#define ITERATIONS 10000
using namespace std;

void deposit(int *balance, sem_t *sem) {
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(sem);
        *balance += 10;
        sem_post(sem);
    }
}

void withdraw(int *balance, sem_t *sem) {
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(sem);
        *balance -= 10;
        sem_post(sem);
    }
}

int main() {
    int shm_fd = shm_open("/balance", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    int *balance = (int *) mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sem_t *sem = sem_open("/semaphore", O_CREAT, 0666, 1);
    *balance = 1000;
    thread deposit_thread(deposit, balance, sem);
    thread withdraw_thread(withdraw, balance, sem);
    deposit_thread.join();
    withdraw_thread.join();
    cout << "Final balance with synchronization: " << *balance << endl;
    munmap(balance, sizeof(int));
    shm_unlink("/balance");
    sem_close(sem);
    sem_unlink("/semaphore");

    return 0;
}
