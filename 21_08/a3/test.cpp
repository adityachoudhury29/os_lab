#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctime>
using namespace std;

bool isPrime(int num) {
    if (num <= 1)
        return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

void findPrimes(int start, int end, int processId) {
    vector<int> primes;
    for (int i = start; i <= end; i++) {
        if (isPrime(i))
            primes.push_back(i);
    }
    cout << "Process " << processId << " found " << primes.size() << " prime numbers." << endl;
    cout << "Primes: "<<endl;
    for(auto it: primes){
        cout<<it<<" ";
    }
    cout<<endl;
}

int main() {
    int N = 100; 
    int M = 3; 

    clock_t start = clock();

    int range = N / M;
    int startNum = 1;
    int endNum = range;

    for (int i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            
            findPrimes(startNum, endNum, pid);
            exit(0);
        } else if (pid > 0) {
            
            startNum = endNum + 1;
            endNum += range;
        } else {
            cerr << "Fork failed." << endl;
            return 1;
        }
    }

    
    for (int i = 0; i < M; i++) {
        wait(NULL);
    }

    clock_t end = clock();
    double timeDiff = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    cout << "Total time taken: " << timeDiff << " seconds." << endl;

    return 0;
}