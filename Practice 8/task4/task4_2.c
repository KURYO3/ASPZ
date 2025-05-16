#include <stdio.h>
#include <unistd.h>

int main() {
    int pid;
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    printf("pid = %d, process ID = %d\n", pid, getpid());

    return 0;
}