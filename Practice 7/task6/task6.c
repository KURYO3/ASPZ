#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#define INITIAL_CAPACITY 32

int is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1)
        return 0;
    return S_ISDIR(st.st_mode);
}

int compare_strings(const void *a, const void *b) {
    const char **str_a = (const char **)a;
    const char **str_b = (const char **)b;
    return strcmp(*str_a, *str_b);
}

int main(void) {
    DIR *dir;
    struct dirent *entry;
    char **dirs = NULL;
    size_t count = 0, capacity = INITIAL_CAPACITY;

    dirs = malloc(capacity * sizeof(char *));
    if (dirs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "./%s", entry->d_name);

        if (is_directory(full_path)) {
            if (count >= capacity) {
                capacity *= 2;
                dirs = realloc(dirs, capacity * sizeof(char *));
                if (dirs == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }
            dirs[count++] = strdup(entry->d_name); 
        }
    }

    closedir(dir);

    qsort(dirs, count, sizeof(char *), compare_strings);

    printf("Subdirectories in alphabetic order:\n");
    for (size_t i = 0; i < count; i++) {
        printf("%s/\n", dirs[i]);
        free(dirs[i]);
    }

    free(dirs);
    return 0;
}
