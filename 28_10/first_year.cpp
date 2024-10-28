#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

struct SharedCounters {
    int first_year_count;
    int final_year_count;
};

void visit_hod_room(SharedCounters *counters, sem_t *hod_room, sem_t *turn) {
    for (int i = 0; i < 5; i++) { // Limiting visits to 5 times for demonstration
        counters->first_year_count++;

        if (counters->final_year_count > 0) {
            sem_wait(turn);  // Wait for alternating access if final year students are present
        }

        sem_wait(hod_room);  // Enter critical section (HOD’s room)
        std::cout << "1st Year Student " << getpid() << " is visiting the HOD’s room." << std::endl;
        sleep(1);            // Simulate time spent in room

        sem_post(hod_room);  // Leave critical section
        counters->first_year_count--;  // Decrease count for 1st year students

        if (counters->final_year_count > 0) {
            sem_post(turn);  // Allow final year to enter if they are waiting
        }
        sleep(1);            // Wait before next visit
    }
}

int main() {
    // Set up shared memory
    int shm_fd = shm_open("/shared_counters", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedCounters));
    SharedCounters *counters = (SharedCounters *)mmap(0, sizeof(SharedCounters), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Access semaphores
    sem_t *hod_room = sem_open("/hod_room", O_CREAT, 0666, 1); // Initialize as available
    sem_t *turn = sem_open("/turn", O_CREAT, 0666, 1);         // 1st year students start

    visit_hod_room(counters, hod_room, turn);

    // Cleanup
    munmap(counters, sizeof(SharedCounters));
    close(shm_fd);
    sem_close(hod_room);
    sem_close(turn);
    return 0;
}
