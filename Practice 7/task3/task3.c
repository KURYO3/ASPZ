#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <word> <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *keyword = argv[1];
    const char *filename = argv[2];
    char line[MAX_LINE];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File open error");
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword) != NULL) {
            fputs(line, stdout);
        }
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
