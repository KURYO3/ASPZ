#include <stdio.h>

void func() {
    printf ("Inside func()\n");
}

int main() {
    func();
    printf ("Back in main ()\n");
    return 0;
}