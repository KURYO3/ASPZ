#include <stdio.h>
#include <stdlib.h>
#include <string .h>

int main() {
  void *ptr = malloc(16);
  if (!ptr) {
    perror ("malloc failed");
    return 1;
  }

  strcpy(ptr, "Hello!");
  printf ("Before free: %s (ptr = %p)\n", (char *)ptr, ptr);

  free(ptr);

  printf ("After free, ptr still holds: %p\n", ptr);
  printf ("After free: %s\n", (char *)ptr);

  return 0;
}