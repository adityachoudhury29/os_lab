#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <csignal>
#include <sys/wait.h>
using namespace std;


bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}


void handle_prime_found(int sig) {
    
}

int main() {
    int x, y;
    pid_t pid1, pid2;

    
    cout << "Enter the range [x, y] (> 1000): ";
    cin >> x >> y;

    
    int mid = x + (y - x) / 2;

    
    signal(SIGUSR1, handle_prime_found);

    
    if ((pid1 = fork()) == 0) {
        for (int i = x; i <= mid; i++) {
            if (is_prime(i)) {
                cout << "Child 1 (PID " << getpid() << ") found prime: " << i << endl;
                kill(getppid(), SIGUSR1);  
                exit(0);
            }
        }
        exit(0);  
    }

    
    if ((pid2 = fork()) == 0) {
        for (int i = mid + 1; i <= y; i++) {
            if (is_prime(i)) {
                cout << "Child 2 (PID " << getpid() << ") found prime: " << i << endl;
                kill(getppid(), SIGUSR1);  
                exit(0);
            }
        }
        exit(0);  
    }

    
    bool found = false;
    while (!found) {
        int status;
        pid_t child_pid = waitpid(-1, &status, WNOHANG);
        if (child_pid > 0 && WIFEXITED(status)) {
            found = true;
            cout << "Parent: Prime found by child with PID " << child_pid << ". Terminating other child." << endl;

            
            if (child_pid == pid1) {
                kill(pid2, SIGTERM);
            } else {
                kill(pid1, SIGTERM);
            }

            break;
        }

        
        pause();
    }

    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    cout << "Parent: All child processes have completed." << endl;
    return 0;
}