#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int ret;

    ret = system("sudo cat /etc/master.passwd");

    if (ret == -1) {
        perror("system");
        return 1;
    } else if (WIFEXITED(ret) && WEXITSTATUS(ret) != 0) {
        printf("Command failed. Maybe need privilegies sudo.\n");
    }

    return 0;
}
