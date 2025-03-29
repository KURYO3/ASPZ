#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_FILE_SIZE 128
#define FILE_NAME "dice_results.txt"

int check_file_size(const char *file_name, size_t max_size) {
    struct stat st;
    if (stat(file_name, &st) == 0) {
        return (st.st_size > max_size);
    }
    return 0;
}

void clear_file(const char *file_name) {
    FILE *file = fopen(file_name, "w");
    if (file != NULL) {
        fclose(file);
        printf("File cleared successfully.\n");
    } else {
        perror("Error clearing file");
    }
}

int main() {
    FILE *file;
    int roll;
    srand(time(NULL));

    file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    printf("Starting dice simulation. Press Ctrl+C to stop.\n");

    while (1) {
        if (check_file_size(FILE_NAME, MAX_FILE_SIZE)) {
            printf("File size exceeded %d bytes.\n", MAX_FILE_SIZE);
            printf("Do you want to clear the file? (y/n): ");

            char response[10];
            fgets(response, sizeof(response), stdin);

            if (strncmp(response, "y", 1) == 0 || strncmp(response, "Y", 1) == 0) {
                clear_file(FILE_NAME);
                file = freopen(FILE_NAME, "a", file);
                if (file == NULL) {
                    perror("Error reopening file");
                    return EXIT_FAILURE;
                }
            } else {
                printf("Exiting simulation.\n");
                break;
            }
        }

        roll = (rand() % 6) + 1;

        printf("Rolling: %d\n", roll);
        fprintf(file, "Rolling: %d\n", roll);
        fflush(file);

        usleep(100000);  // 100 мс
    }

    fclose(file);
    printf("Simulation finished.\n");
    return EXIT_SUCCESS;
}
