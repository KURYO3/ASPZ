#include <stdio.h>
#include <stdlib.h>

int main() {
  void *ptr1 = realloc (NULL, 16);
  if (ptr1) {
    printf ("realloc (NULL, 16) successful: ptr1 = %p\n", ptr1);
  } else {
    printf ("realloc (NULL, 16) failed\n");
  }

  void *ptr2 = malloc(32);
  if (!ptr2) {
    perror ("malloc failde");
    return 1;
  }

  printf ("Initial allocation: ptr2 = %p\n", ptr2);

  void *ptr3 = realloc(ptr2, 0);
  printf ("realloc(ptr2, 0) returned: ptr3 = %p\n", ptr3) ;

  if (ptr3) {
    free(ptr3);
  }

  return 0;
}