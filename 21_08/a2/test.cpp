#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <ctime>
using namespace std;

void log_pid_ppid(const string &filename) {
    pid_t pid = getpid();
    pid_t ppid = getppid();

    
    time_t now = time(0);
    char* dt = ctime(&now);

    
    ofstream logfile;
    logfile.open(filename, ios_base::app);
    logfile << "Timestamp: " << dt;
    logfile << "PID: " << pid << ", PPID: " << ppid << endl;
    logfile.close();

    
    cout << "PID: " << pid << ", PPID: " << ppid << endl;
}

int main() {
    int n;
    cout << "Enter the number of child processes to create: ";
    cin >> n;

    string filename = "log.txt";

    
    ofstream logfile(filename);
    logfile << "Process Log" << endl;
    logfile.close();

    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Fork failed!" << endl;
            return 1;
        }

        if (pid == 0) {
            
            log_pid_ppid(filename);
            sleep(60); 
            return 0; 
        }
        
    }

    
    log_pid_ppid(filename);
    sleep(60); 

    
    while (wait(NULL) > 0);

    return 0;
}
