#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int file = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file < 0) {
        perror("open");
        return 1;
    }

    int saved_stdout = dup(STDOUT_FILENO);

    dup2(file, STDOUT_FILENO);

    printf("Hello dup()\n");

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    int file2 = open("output.txt", O_WRONLY | O_APPEND);
    if (file2 < 0) {
        perror("open 2");
        return 1;
    }

    dup2(file2, STDOUT_FILENO);

    printf("Hello dup2()\n");

    close(file2);

    return 0;
}
