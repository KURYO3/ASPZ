#include <stdio.h>
#include <stdlib.h>

#define LINES_PER_PAGE 20
#define MAX_LINE 1024

void show_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return;
    }

    char line[MAX_LINE];
    int line_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        fputs(line, stdout);
        line_count++;

        if (line_count >= LINES_PER_PAGE) {
            printf("== Press Enter for continue == ");
            while (getchar() != '\n');
            line_count = 0;
        }
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2] ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        printf("\n--- File output: %s ---\n", argv[i]);
        show_file(argv[i]);
    }

    return EXIT_SUCCESS;
}
