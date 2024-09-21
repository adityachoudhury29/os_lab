#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid < 0) {  
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {  
        
        printf("Child running……mypid %d myparent pid %d mygroupid %d\n",
               getpid(), getppid(), getgid());

        
        printf("Child: Bye\n");
    } else {  
        
        printf("Parent running….. mypid %d myparents pid %d mygroupid %d\n",
               getpid(), getppid(), getgid());

        
        printf("Parent: Bye\n");
    }

    return 0;
}
