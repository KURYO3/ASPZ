#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

volatile sig_atomic_t got_alarm = 0;

void alarm_handler(int signo) {
    got_alarm = 1;
}

int main(void) {
    struct sigaction sa;
    struct itimerval its;
    struct timespec start, end;
    double elapsed;
    int i;

    printf("Testing timer accuracy (1..60 seconds)\n");

    sa.sa_handler = alarm_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    for (i = 1; i <= 60; ++i) {
        got_alarm = 0;

        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
            perror("clock_gettime");
            return 1;
        }

        its.it_value.tv_sec = i;
        its.it_value.tv_usec = 0;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_usec = 0;

        if (setitimer(ITIMER_REAL, &its, NULL) == -1) {
            perror("setitimer");
            return 1;
        }

        while (!got_alarm)
            pause();

        if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
            perror("clock_gettime");
            return 1;
        }

        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Requested: %2d s, Elapsed: %.6f s\n", i, elapsed);
    }

    return 0;
}
