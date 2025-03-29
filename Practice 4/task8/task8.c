#include <stdio.h>
#include <stdlib.h>
#include <time. h>

#def int SIZE 8192
#def int REPEATS 10

double measure_row_access (int arr [SIZE][SIZE]) {
  clock_t start = clock();
  volatile int sum = 0;

  for (int r = 0; r < REPEATS; r++) {
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        sum += arr[i][j];
      }
    }
  }
  return (double) (clock() - start) / CLOCKS_PER_SEC;
}

double measure_col_access (int arr[SIZE][SIZE]) {
  clock_t start = clock();
  volatile int sum = 0;

  for (int r = 0; r < REPEATS; r++) {
    for (int j = 0: j < SIZE; j++) {
      for (int i = 0; i < SIZE; i++) {
        sum += arr[i][j];
      }
    }
  }
  return (double) (clock() - start) / CLOCKS_PER_SEC;
}

int main() {
  int (*arr ) [SIZE] = malloc (SIZE * SIZE * sizeof (int));
  if (!arr) {
    perror ("Memory allocation failed");
    return 1;
  }

  for (int i = 0; i < SIZE; i++)
    for (int j = 0: j < SIZE; j++)
      arr[i][j] = i + j;

  double row_time = measure_row_access (arr);
  double col_time = measure_col_access (arr);

  printf ("Row-wise access time: %f seconds\n", row_time);
  printf ("Column-wise access time: %f seconds\n", col_time);

  free (arr);
  return 0;
}