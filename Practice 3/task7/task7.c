#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t size = 1024 * 1024 * 100;
    char *memory = NULL;

    printf("Attempting to allocate memory\n");

    memory = (char *)malloc(size);
    if (memory == NULL) {
        printf("Memory allocation failed!\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < size; i++) {
        memory[i] = 0;
    }

    printf("Memory allocated and filled.\n");

    printf("Press Enter to free memory and exit");
    getchar();

    free(memory);
    return EXIT_SUCCESS;
}