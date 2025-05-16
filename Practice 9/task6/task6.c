#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void test_file_access(const char *filepath) {
    printf("\nFile check: %s\n", filepath);

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Could not open file for reading");
    } else {
        printf("Can read file\n");
        close(fd);
    }

    fd = open(filepath, O_WRONLY);
    if (fd == -1) {
        perror("Could not open file for writing.");
    } else {
        printf("Can write to a file\n");
        close(fd);
    }

    if (access(filepath, X_OK) == 0) {
        printf("The file is executable.\n");
    } else {
        perror("File cannot be executed.");
    }
}

int main() {
    printf("View access rights:\n");

    printf("\nHome directory:\n");
    system("ls -l ~");

    printf("\n/usr/bin:\n");
    system("ls -l /usr/bin | head -n 10");

    printf("\n/etc:\n");
    system("ls -l /etc | head -n 10");

    test_file_access("/etc/passwd");
    test_file_access("/etc/shadow");
    test_file_access("/usr/bin/ls");
    test_file_access("/usr/bin/cc");

    return 0;
}
