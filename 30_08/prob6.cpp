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


void search_and_write_primes(int start, int end, const string& filename) {
    ofstream file(filename, ios::app); 
    if (!file.is_open()) {
        cerr << "Failed to open file in child" << endl;
        exit(1);
    }

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            file << i << endl;
        }
    }
    exit(0);  
}

int main() {
    int x, y, n;

    
    cout << "Enter the range [x, y] (both > 1000): ";
    cin >> x >> y;

    cout << "Enter the number of child processes: ";
    cin >> n;

    string filename = "primes1.txt";

    
    int range_per_child = (y - x + 1) / n;
    int remainder = (y - x + 1) % n;

    vector<pid_t> pids;

    for (int i = 0; i < n; i++) {
        int start = x + i * range_per_child;
        int end = start + range_per_child - 1;

        
        if (i == n - 1) {
            end += remainder;
        }

        pid_t pid = fork();
        if (pid == 0) {  
            search_and_write_primes(start, end, filename);
        } else if (pid < 0) {
            cerr << "Fork failed" << endl;
            return 1;
        } else {
            pids.push_back(pid);
        }
    }

    
    for (pid_t pid : pids) {
        waitpid(pid, nullptr, 0);
    }

    cout << "All child processes completed. Prime numbers are written to " << filename << "." << endl;
    return 0;
}