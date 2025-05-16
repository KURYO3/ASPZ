#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 100000

int compare(const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;
    return (x - y);
}

int is_sorted(int *arr, int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i - 1] > arr[i])
            return 0;
    }
    return 1;
}

void fill_sorted(int *arr, int size) {
    for (int i = 0; i < size; i++)
        arr[i] = i;
}

void fill_reverse_sorted(int *arr, int size) {
    for (int i = 0; i < size; i++)
        arr[i] = size - i;
}

void fill_constant(int *arr, int size) {
    for (int i = 0; i < size; i++)
        arr[i] = 42;
}

void fill_patterned(int *arr, int size) {
    for (int i = 0; i < size; i++)
        arr[i] = (i % 5);
}

void fill_random(int *arr, int size) {
    for (int i = 0; i < size; i++)
        arr[i] = rand();
}

double timed_qsort(int *arr, int size) {
    clock_t start = clock();
    qsort(arr, size, sizeof(int), compare);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

void run_test(const char *name, void (*fill_func)(int *, int)) {
    int *arr = malloc(SIZE * sizeof(int));
    fill_func(arr, SIZE);

    printf("Test: %s\n", name);
    double time = timed_qsort(arr, SIZE);
    printf("  Time of sorting: %.5f секунд\n", time);
    printf("  Sorting is corect: %s\n\n", is_sorted(arr, SIZE) ? "yes" : "no");

    free(arr);
}

int main() {
    srand(time(NULL));

    run_test("Already sorted array", fill_sorted);
    run_test("Reverse array", fill_reverse_sorted);
    run_test("Identical elements", fill_constant);
    run_test("Repeating pattern", fill_patterned);
    run_test("Random array", fill_random);

    return 0;
}
