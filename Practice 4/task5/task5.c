#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  size_t big_size = 1024L * 1024L * 1024L * 1024L;
  void *ptr = malloc(1024);

  if (!ptr) {
    perror ("Initial malloc failed");
    return 1;
  }

  printf ("Initial allocation successful, ptr = %p\n", ptr);

  void *new_ptr = realloc(ptr, big_size);

  if (!new_ptr) {
    perror ("realloc failed");
    printf ("ptr is still valid: %p\n", ptr);
  } else {
    ptr = new_ptr;
    printf ("Realloc successful, new ptr = %p\n", ptr);

    memset (ptr, 0, big_size);
    printf ("Memory initialized successfully\n");
  }

  free(ptr);
  return 0;
}