#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10
using namespace std;

struct SharedBuffer {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
};


void produceItem(SharedBuffer *shared_buffer, int item, sem_t *empty, sem_t *full, sem_t *mutex) {
    sem_wait(empty);
    sem_wait(mutex);

    
    shared_buffer->buffer[shared_buffer->in] = item;
    cout << "Producer " << getpid() << " produced item: " << item 
              << " at position " << shared_buffer->in << endl;
    shared_buffer->in = (shared_buffer->in + 1) % BUFFER_SIZE;

    sem_post(mutex);
    sem_post(full);
}

int main() {
    srand(time(0) ^ getpid());

    
    int shm_fd = shm_open("/shared_buffer", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedBuffer));
    SharedBuffer *shared_buffer = (SharedBuffer *)mmap(0, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    
    sem_t *empty = sem_open("/empty", O_CREAT, 0666, BUFFER_SIZE);
    sem_t *full = sem_open("/full", O_CREAT, 0666, 0);
    sem_t *mutex = sem_open("/mutex", O_CREAT, 0666, 1);

    
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        int item = rand() % 100; 
        produceItem(shared_buffer, item, empty, full, mutex);

        sleep(rand() % 3); 
    }

    
    munmap(shared_buffer, sizeof(SharedBuffer));
    close(shm_fd);
    return 0;
}
