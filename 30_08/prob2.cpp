#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void create_binary_tree(int depth);
void sigint_handler(int sig);

int main() {
    int n;

    cout << "Enter the depth of the binary tree: ";
    cin >> n;

    
    signal(SIGINT, SIG_IGN);

    
    setpgid(0, 0);

    
    create_binary_tree(n);

    
    while (true) {
        pause(); 
    }

    return 0;
}

void create_binary_tree(int depth) {
    if (depth == 0) return;

    pid_t left_child, right_child;

    left_child = fork();
    if (left_child == 0) { 
        
        signal(SIGINT, sigint_handler);

        
        create_binary_tree(depth - 1);

        
        while (true) {
            pause(); 
        }
    } else {
        right_child = fork();
        if (right_child == 0) { 
            
            signal(SIGINT, sigint_handler);

            
            create_binary_tree(depth - 1);

            
            while (true) {
                pause(); 
            }
        }
    }
}

void sigint_handler(int sig) {
    
    cout << "Process " << getpid() << " terminated" << endl;
    exit(0);
}
