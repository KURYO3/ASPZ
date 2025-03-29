#include <stdio.h>
#include <stdlib.h>

void recursive_func(int depth) {
    printf("Recursion depth: %d\n", depth);
    recursive_func(depth + 1);
}

int main() {
    printf("Starting recursive function...\n");
    recursive_func(1);
    return EXIT_SUCCESS;
}