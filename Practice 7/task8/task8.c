#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define MAX_INPUT 10
#define PATH_MAX_LEN 1024

int is_regular_file(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1)
        return 0;
    return S_ISREG(st.st_mode);
}

int main(void) {
    DIR *dir;
    struct dirent *entry;
    char input[MAX_INPUT];

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (is_regular_file(entry->d_name)) {
            printf("Delete file '%s'? (y/n/a - all): ", entry->d_name);
            if (fgets(input, sizeof(input), stdin) == NULL)
                continue;

            if (input[0] == 'y' || input[0] == 'Y') {
                if (remove(entry->d_name) == 0) {
                    printf("Deleted: %s\n", entry->d_name);
                } else {
                    perror(entry->d_name);
                }
            } else if (input[0] == 'a' || input[0] == 'A') {
                do {
                    if (is_regular_file(entry->d_name)) {
                        if (remove(entry->d_name) == 0) {
                            printf("Deleted: %s\n", entry->d_name);
                        } else {
                            perror(entry->d_name);
                        }
                    }
                } while ((entry = readdir(dir)) != NULL);
                break;
            } else {
                printf("Skipped: %s\n", entry->d_name);
            }
        }
    }

    closedir(dir);
    return 0;
}

// Написати що функція "всі" не використовувалась бо шкода файли