#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

int main(void) {
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;
    sigset_t mask;
    siginfo_t si;
    struct timespec start, end;
    int i, signo;

    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;

    if (timer_create(CLOCK_MONOTONIC, &sev, &timerid) == -1) {
        perror("timer_create");
        return EXIT_FAILURE;
    }

    printf("Testing POSIX timer accuracy (1..60 seconds)\n");

    for (i = 1; i <= 60; i++) {
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
            perror("clock_gettime");
            break;
        }

        its.it_value.tv_sec = i;
        its.it_value.tv_nsec = 0;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;

        if (timer_settime(timerid, 0, &its, NULL) == -1) {
            perror("timer_settime");
            break;
        }

        signo = sigwaitinfo(&mask, &si);
        if (signo == -1) {
            perror("sigwaitinfo");
            break;
        }

        if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
            perror("clock_gettime");
            break;
        }

        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Requested: %2d s, Elapsed: %.6f s\n", i, elapsed);
    }

    timer_delete(timerid);
    return EXIT_SUCCESS;
}
