#include <stdio.h>

void func() {
    int arr1[10000];
    int arr2[10000];
    printf ("Stack top after allocation: %p\n", &arr2);
}

int main() {
    int i;
    printf ("Initial stack top: %p\n", &i);
    func();
    return 0;
}