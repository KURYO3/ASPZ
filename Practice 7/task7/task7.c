#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>

#define MAX_PATH 1024

int is_executable_owned(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == -1)
        return 0;

    uid_t uid = getuid();
    return S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR) && (st.st_uid == uid);
}

void process_file(const char *filename) {
    char response[10];
    struct stat st;

    if (stat(filename, &st) == -1) {
        perror(filename);
        return;
    }

    printf("FileL: %s — get permission for reading for another users? (y/n): ", filename);
    if (fgets(response, sizeof(response), stdin) == NULL)
        return;

    if (response[0] == 'y' || response[0] == 'Y') {
        mode_t new_mode = st.st_mode | S_IROTH;
        if (chmod(filename, new_mode) == -1) {
            perror("chmod");
        } else {
            printf("Permission for reading is granted for: %s\n", filename);
        }
    } else {
        printf("Skipped: %s\n", filename);
    }
}

int main(void) {
    DIR *dir = opendir(".");
    struct dirent *entry;

    if (!dir) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    printf("Searching for executed C-programs in current directory:\n");

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (is_executable_owned(entry->d_name)) {
            process_file(entry->d_name);
        }
    }

    closedir(dir);
    return EXIT_SUCCESS;
}
