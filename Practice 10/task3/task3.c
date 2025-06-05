#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;
    int level = 0;
    const int MAX_LEVEL = 5;

    while (level < MAX_LEVEL) {
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            level++;
            continue;
        } else {
            waitpid(pid, &status, 0);
            printf("Parent level %d PID %d: child (PID %d) at level %d has exited.\n",
                   level, getpid(), pid, level + 1);
            fflush(stdout);
            exit(0);
        }
    }
    printf("Leaf level %d PID %d: no more children, exiting.\n", level, getpid());
    fflush(stdout);
    sleep(1);
    return 0;
}
