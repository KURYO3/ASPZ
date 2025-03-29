#include <stdio.h>
#include <stdlib.h>

int main() {
  void *ptr = malloc (0);

  if (ptr == NULL) {
    printf ("malloc(0) return NULL\n");
  } else {
    printf ("malloc(0) return pointer: %p\n", ptr);
    free(ptr);
  }

  return 0;
}