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
    for (int i = 0; i < 5; i++) { 
        counters->final_year_count++;

        if (counters->first_year_count > 0) {
            sem_wait(turn);  
        }

        sem_wait(hod_room);  
        std::cout << "Final Year Student " << getpid() << " is visiting the HOD’s room." << std::endl;
        sleep(1);            

        sem_post(hod_room);  
        counters->final_year_count--;  

        if (counters->first_year_count > 0) {
            sem_post(turn);  
        }
        sleep(1);            
    }
}

int main() {
    
    int shm_fd = shm_open("/shared_counters", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedCounters));
    SharedCounters *counters = (SharedCounters *)mmap(0, sizeof(SharedCounters), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    
    sem_t *hod_room = sem_open("/hod_room", O_CREAT, 0666, 1); 
    sem_t *turn = sem_open("/turn", O_CREAT, 0666, 0);         

    visit_hod_room(counters, hod_room, turn);

    
    munmap(counters, sizeof(SharedCounters));
    close(shm_fd);
    sem_close(hod_room);
    sem_close(turn);
    return 0;
}
