#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
using namespace std;

int x, y;  


bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}


void handle_sigusr1(int sig) {
    cout << "Child 1 found a prime number." << endl;
    
    kill(0, SIGTERM);
}


void handle_sigusr2(int sig) {
    cout << "Child 2 found a prime number." << endl;
    
    kill(0, SIGTERM);
}

void find_prime(int start, int end, int sig_done) {
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            cout << "Process " << getpid() << " found prime number: " << i << endl;
            kill(getppid(), sig_done);  
            exit(0);  
        }
    }
    exit(0);  
}

int main() {
    
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    
    cout << "Enter the range [x, y]: ";
    cin >> x >> y;

    pid_t pid1, pid2;

    int mid = x + (y - x) / 2;

    
    if ((pid1 = fork()) == 0) {
        
        find_prime(x, mid, SIGUSR1);
    } else {
        
        if ((pid2 = fork()) == 0) {
            
            find_prime(mid + 1, y, SIGUSR2);
        }
    }

    
    while (true) {
        pause();  
    }

    return 0;
}