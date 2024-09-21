#include <iostream>
#include <fstream>
#include <unistd.h>
#include <csignal>
#include <fcntl.h>
#include <string>
using namespace std;
#define FILENAME "primes.txt"

int x, y;  


bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}


void find_primes(int start, int end, int sig_done) {
    ofstream file(FILENAME, ios::app);
    if (!file.is_open()) {
        perror("File open error");
        exit(1);
    }

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            file << "[" << getpid() << ", " << i << "]\n";
        }
    }

    file.close();

    
    kill(getppid(), sig_done);

    
    while (true) {
        pause();
    }
}


void handle_sigusr1(int sig) {
    cout << "Child 1 (SIGUSR1) has completed its task." << endl;
}


void handle_sigusr2(int sig) {
    cout << "Child 2 (SIGUSR2) has completed its task." << endl;
}

int main() {
    
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    
    cout << "Enter the range [x, y]: ";
    cin >> x >> y;

    pid_t pid1, pid2;

    
    if ((pid1 = fork()) == 0) {
        
        int mid = x + (y - x) / 2;
        find_primes(x, mid, SIGUSR1);
    } else {
        
        if ((pid2 = fork()) == 0) {
            
            int mid = x + (y - x) / 2 + 1;
            find_primes(mid, y, SIGUSR2);
        }
    }

    
    while (true) {
        pause();  
    }

    return 0;
}