#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
using namespace std;


bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}


void search_and_write_primes(int start, int end, std::ofstream& file, pid_t pid) {
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            file << "[" << pid << ", " << i << "]\n";
        }
    }
    exit(0);  
}

int main() {
    int x, y;
    pid_t pid1, pid2;
    int status;

    
    std::cout << "Enter the range [x, y] (> 1000): ";
    std::cin >> x >> y;

    
    std::ofstream file("primes_with_pids.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to create file" << std::endl;
        return 1;
    }

    
    int mid = x + (y - x) / 2;

    
    if ((pid1 = fork()) == 0) {
        search_and_write_primes(x, mid, file, getpid());
    }

    
    if ((pid2 = fork()) == 0) {
        search_and_write_primes(mid + 1, y, file, getpid());
    }

    
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    
    file.close();

    cout << "Parent: All child processes have completed. Prime numbers are written to 'primes_with_pids.txt'." << std::endl;
    return 0;
}