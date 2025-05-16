#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Running whoami command:\n");
    int ret1 = system("whoami");
    if (ret1 == -1) {
        perror("Error executing whoami");
    }

    printf("Running id command:\n");
    int ret2 = system("id");
    if (ret2 == -1) {
        perror("Error executing id");
    }

    return 0;
}