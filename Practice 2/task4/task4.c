#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define MSG &quot;In function %20s; &amp;localvar = %p\n&quot;

static void bar_is_now_closed(void) {
    int localvar = 5;
    printf(MSG, FUNCTION, &localvar);
    printf("\n Now blocking on pause()...\n");

    pause();
}

static void bar(void) {
    int localvar = 5;
    printf(MSG, FUNCTION, &localvar);
    bar_is_now_closed();
}

static void foo(void) {
    int localvar = 5;
    printf(MSG, FUNCTION, &localvar);
    bar();
}

int main(int argc, char **argv) {
    int localvar = 5;
    printf(MSG, FUNCTION, &localvar);
    foo();
    exit(EXIT_SUCCESS);
}