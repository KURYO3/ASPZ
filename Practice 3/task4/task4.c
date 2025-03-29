#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#define NUM1 7
#define MAX1 49

#define NUM2 6
#define MAX2 36

void select_random_numbers(int *numbers, int max, int count) {
    for (int i = 0; i < max; i++) {
        numbers[i] = i + 1;
    }
    for (int i = max - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }
}

void print_numbers(int *numbers, int count) {
    for (int i = 0; i < count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
}

int main() {
    struct rlimit rl;
    rl.rlim_cur = 2;  // Ліміт на час ЦП (2 секунди)
    rl.rlim_max = 2;  // Максимальний ліміт часу ЦП
    if (setrlimit(RLIMIT_CPU, &rl) != 0) {
        perror("Error setting CPU time limit");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    int numbers_49[MAX_49];
    select_random_numbers(numbers_49, MAX_49, NUM_49);
    printf("Lottery numbers (7 out of 49): ");
    print_numbers(numbers_49, NUM_49);

    int numbers_36[MAX_36];
    select_random_numbers(numbers_36, MAX_36, NUM_36);
    printf("Lottery numbers (6 out of 36): ");
    print_numbers(numbers_36, NUM_36);

    printf("Lottery generation completed successfully.\n");

    return EXIT_SUCCESS;
}