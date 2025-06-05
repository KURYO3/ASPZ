#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        _exit(7);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
