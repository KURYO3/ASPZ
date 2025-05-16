#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

long time_diff_ms(struct timeval start, struct timeval end) {
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    return (seconds * 1000) + (useconds / 1000);
}

int main(void) {
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for (volatile long i = 0; i < 100000000; ++i);

    gettimeofday(&end, NULL);

    printf("Time of execution: %ld мс\n", time_diff_ms(start, end));

    return 0;
}
