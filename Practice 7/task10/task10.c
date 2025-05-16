#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float rand_0_1() {
    return (float)rand() / (float)RAND_MAX;
}

float rand_0_n(float n) {
    return rand_0_1() * n;
}

int main(int argc, char *argv[]) {
    float n = 1.0;

    if (argc == 2) {
        n = atof(argv[1]);
        if (n <= 0.0) {
            fprintf(stderr, "n must be a positive float number\n");
            return EXIT_FAILURE;
        }
    }

    srand((unsigned int)time(NULL));

    printf("\n(a) From 0.0 to 1.0:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %.6f\n", rand_0_1());
    }

    printf("\n(b) From 0.0 to %.2f:\n", n);
    for (int i = 0; i < 5; i++) {
        printf("  %.6f\n", rand_0_n(n));
    }

    return 0;
}
