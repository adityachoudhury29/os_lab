#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void logProcessInfo(int pid, int ppid) {
    ofstream logFile("log.txt", ios::app);
    if (logFile.is_open()) {
        logFile << "PID: " << pid << ", PPID: " << ppid << ", Timestamp: " << time(nullptr) << endl;
        logFile.close();
    }
}

void createProcessHierarchy(int depth, int pid, int ppid) {
    logProcessInfo(pid, ppid);
    sleep(10);

    if (depth > 0) {
        pid_t child1 = fork();
        if (child1 == 0) {
            createProcessHierarchy(depth - 1, getpid(), getppid());
            exit(0);
        }

        pid_t child2 = fork();
        if (child2 == 0) {
            createProcessHierarchy(depth - 1, getpid(), getppid());
            exit(0);
        }

        waitpid(child1, nullptr, 0);
        waitpid(child2, nullptr, 0);
    }
}

int main() {
    int depth;
    cout << "Enter the depth of the full binary tree: ";
    cin >> depth;

    ofstream logFile("log.txt");
    if (!logFile.is_open()) {
        cerr << "Failed to create log file!" << endl;
        return 1;
    }
    logFile.close();

    createProcessHierarchy(depth, getpid(), getppid());

    return 0;
}