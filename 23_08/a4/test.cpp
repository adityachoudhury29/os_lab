#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>
using namespace std;

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

void searchPrimeInRange(int start, int end, pid_t parentPid) {
    for (int i = start; i <= end; i++) {
        if (isPrime(i)) {
            cout << "Prime " << i << " found by process " << getpid() << endl;
            kill(parentPid, SIGUSR1); 
            return;
        }
    }
}

void signalHandler(int sig) {
    
}

int main() {
    int N = 100; 
    int M = 5; 
    pid_t parentPid = getpid();

    signal(SIGUSR1, signalHandler); 

    int rangePerProcess = N / M;
    vector<pid_t> childPids;

    for (int i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) { 
            int start = i * rangePerProcess + 1;
            int end = (i + 1) * rangePerProcess;
            if (i == M - 1) end = N; 
            searchPrimeInRange(start, end, parentPid);
            exit(0); 
        } else if (pid > 0) { 
            childPids.push_back(pid);
        } else {
            cerr << "Fork failed" << endl;
            return 1;
        }
    }

    
    for (pid_t pid : childPids) {
        waitpid(pid, nullptr, 0);
    }

    return 0;
}