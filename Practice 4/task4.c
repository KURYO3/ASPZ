#include <stdio.h>
#include <stdlib.h>

int main() {
  void *ptr = NULL;
  for (int i = 0; i < 3; i++) {
    if (!ptr)
      ptr = malloc (10);
    if (!ptr) {
      perror ("malloc failed");
      return 1;
    }

    printf ("Iteration %d: ptr = %p\n", i, ptr);
    free(ptr);
  }
  return 0;
}