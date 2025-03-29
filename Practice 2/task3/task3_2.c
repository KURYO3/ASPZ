#include <stdio.h>
#include <stdlib.h>

int x = 1;
int y;

void func() {
    int z;
    int *heap = malloc(sizeof (int));

    printf ("Text segment (code): %p\n", (void*)func);
    printf ("Initialized data segmant: %p\n", (void*)&x);
    printf ("BSS segmant: %p\n", (void*)&y);
    printf ("Heap segment: %p\n", (void*)heap);
    printf ("Stack segment: %p\n", (void*) &z);

    free (heap);
}

int main() {
    func();
    return 0;
}