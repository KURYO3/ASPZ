#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int check_rw(const char *filename) {
    printf("Check file access as current user\n");

    if (access(filename, R_OK) == 0) {
        printf("Can read file\n");
    } else {
        printf("Can't read file: %s\n", strerror(errno));
    }

    if (access(filename, W_OK) == 0) {
        printf("Can write file\n");
    } else {
        printf("Can't write file: %s\n", strerror(errno));
    }

    return 0;
}

int main() {
    const char *filename = "/tmp/testfile_tmp.txt";

    printf("UID of current user: %d\n", getuid());

    int fd = open(filename, O_CREAT | O_WRONLY, 0600);
    if (fd == -1) {
        perror("File creation error");
        return 1;
    }

    write(fd, "This is a temp file.\n", 21);
    close(fd);
    printf("File %s created.\n", filename);

    printf("\nChange owner to root and permissions to 644\n");

    char cmd1[256];
    char cmd2[256];
    snprintf(cmd1, sizeof(cmd1), "sudo chown root:wheel %s", filename);
    snprintf(cmd2, sizeof(cmd2), "sudo chmod 644 %s", filename);

    if (system(cmd1) != 0) {
        printf("Error executing chown. You may not have sudo privileges.\n");
    }
    if (system(cmd2) != 0) {
        printf("Error executing chmod. You may not have sudo privileges.\n");
    }

    printf("\nChecking access after changing rights\n");
    check_rw(filename);

    return 0;
}
