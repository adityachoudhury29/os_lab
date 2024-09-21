#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main() {
    int n = 5;
    pid_t pid;

    for (int i = 0; i < n; ++i) {
        pid = fork();

        if (pid == 0) {
            cout << "Child Process: PID=" << getpid() << ", PPID=" << getppid() << ", GRPID=" << getpgid(0) << endl;
            
            if (setpgid(0, 0) == -1) perror("setpgid error");

            while (true);
        }
        else if (pid > 0) {
        }
        else {
            cerr << "Fork failed!" << endl;
            return 1;
        }
    }

    if (pid > 0) {
        cout << "Parent Process: PID=" << getpid() << ", GRPID=" << getpgid(0) << endl;
        sleep(3);
        kill(0, SIGKILL);
    }

    return 0;
}
