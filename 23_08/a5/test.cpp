#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
using namespace std;

volatile sig_atomic_t helloCount = 0;

void handle_sigusr1(int sig) {
    cout << "Parent: World" << endl;
}

void handle_sigusr2(int sig) {
    cout << "Parent: Hello" << endl;
    helloCount++;
}

int main() {
    srand(time(NULL)); 
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    pid_t pid = fork();

    if (pid == 0) { 
        while (true) {
            sleep(5); 
            int x = rand() % 2; 
            if (x == 0) {
                cout << "Child: Hello" << endl;
                kill(getppid(), SIGUSR1);
            } else {
                cout << "Child: World" << endl;
                kill(getppid(), SIGUSR2);
            }
        }
    } else if (pid > 0) { 
        while (helloCount < 5) {
            pause(); 
        }
        kill(pid, SIGTSTP); 
        wait(NULL); 
    } else {
        cerr << "Fork failed" << endl;
        return 1;
    }

    return 0;
}