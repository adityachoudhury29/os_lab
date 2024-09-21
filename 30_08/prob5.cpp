#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <ctime>
#include <csignal>
using namespace std;

#define N 1000000000  


void search_array(vector<int>& arr, int start, int end, int target) {
    for (int i = start; i < end; i++) {
        if (arr[i] == target) {
            cout << "Child " << getpid() << " found the element at index " << i << "." << endl;
            exit(1);  
        }
    }
    exit(0);  
}

int main() {
    vector<int> arr(N);
    int n, target;

    
    srand(time(nullptr));
    for (int& val : arr) {
        val = rand() % N;
    }

    
    cout << "Enter the number of child processes: ";
    cin >> n;

    
    cout << "Enter the element to search: ";
    cin >> target;

    
    int elements_per_child = N / n;
    int remainder = N % n;

    pid_t pid;
    bool found = false;

    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) {  
            int start = i * elements_per_child;
            int end = start + elements_per_child;
            if (i == n - 1) {  
                end += remainder;
            }
            search_array(arr, start, end, target);
        } else if (pid < 0) {
            cerr << "Fork failed" << endl;
            exit(1);
        }
    }

    
    int status;
    for (int i = 0; i < n; i++) {
        pid = wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            cout << "Parent: Child " << pid << " found the element. Terminating other children." << endl;
            found = true;
            break;
        }
    }

    
    if (found) {
        for (int i = 0; i < n; i++) {
            kill(0, SIGTERM);  
        }
    }

    return 0;
}