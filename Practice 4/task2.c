#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

int main() {
  int x = -1;
  void *neg_ptr = malloc(x) ;
  if (neg_ptr == NULL) {
    perror ("malloc can't allocate memory for negative size\n");
  } else {
    printf ("malloc succesfully allocate memory for negative size\n");
    free(neg_ptr);
  }

  int xa = INT_MAX;
  int xb = 2;
  int num = xa * xb;
  printf ("xa: %d, xb: %d, num: %d\n", xa, xb, num) ;

  void *overf low_ptr = malloc (num);
  if (overf low_ptr == NULL) {
    perror ("malloc can't allocate memory durind overf low\n");
  } else {
    printf ("malloc succesfully allocate memory during overf low\n");
    free(overflow_ptr);
  }

  size_t xa1 = SIZE_MAX / 2;
  size_t xb1 = 3;
  size_t num1 = xa1 * xb1;
  printf ("xa1: %zu, xb1: %zu, num1: %zu\n", xa1, xb1, num1) ;

  void *overflow_ptr1 = malloc (num1);
  if (overf low_ptr1 == NULL) {
    perror ("malloc can't allocate memory during overflow\n");
  } else {
    printf ("malloc succesfully allocate memory during overf low\n");
    free(overf low_ptr1);
  }

  return 0;
}