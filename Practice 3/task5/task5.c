#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_FILE_SIZE 1048576

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Program need two arguments\n");
        return EXIT_FAILURE;
    }

    const char *source_file = argv[1];
    const char *destination_file = argv[2];

    FILE *source = fopen(source_file, "rb");
    if (source == NULL) {
        printf("Cannot open file %s for reading\n", source_file);
        return EXIT_FAILURE;
    }

    FILE *destination = fopen(destination_file, "wb");
    if (destination == NULL) {
        fclose(source);
        printf("Cannot open file %s for writing\n", destination_file);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    size_t total_written = 0;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, source)) > 0) {
        if (total_written + bytes_read > MAX_FILE_SIZE) {
            printf("Error: File size limit exceeded while writing to %s\n", destination_file);
            fclose(source);
            fclose(destination);
            return EXIT_FAILURE;
        }

        size_t bytes_written = fwrite(buffer, 1, bytes_read, destination);
        if (bytes_written != bytes_read) {
            printf("Error writing to file %s\n", destination_file);
            fclose(source);
            fclose(destination);
            return EXIT_FAILURE;
        }

        total_written += bytes_written;
    }

    fclose(source);
    fclose(destination);

    printf("File successfully copied from %s to %s\n", source_file, destination_file);
    return EXIT_SUCCESS;
}