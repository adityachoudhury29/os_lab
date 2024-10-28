#include <iostream>
#include <thread>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#define ITERATIONS 10000
using namespace std;

void deposit(int *balance) {
    for (int i = 0; i < ITERATIONS; i++) {
        *balance += 10;
    }
}

void withdraw(int *balance) {
    for (int i = 0; i < ITERATIONS; i++) {
        *balance -= 10;
    }
}

int main() {
    
    int shm_fd = shm_open("/balance1", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    int *balance = (int *) mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    
    *balance = 1000;

    
    thread deposit_thread(deposit, balance);
    thread withdraw_thread(withdraw, balance);

    
    deposit_thread.join();
    withdraw_thread.join();

    
    cout << "Final balance without synchronization: " << *balance << endl;

    
    munmap(balance, sizeof(int));
    shm_unlink("/balance1");

    return 0;
}