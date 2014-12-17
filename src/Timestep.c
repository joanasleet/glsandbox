#include "Timestep.h"

#include "common.h"

#ifdef _POSIX_C_SOURCE
#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif
#endif

#include <time.h>

static double start, current, elapsed;
// static struct timespec sleepPeriod;

void startTimer() {
    start = glfwGetTime();
}

double elapsedTime() {
    current = glfwGetTime();
    elapsed = current - start;
    start = current;
    return elapsed;
}

void wait(long nanosec) {

    // undefined in c99
    // sleepPeriod.tv_sec = 0;
    // sleepPeriod.tv_nsec = nanosec;

    // nanosleep(&sleepPeriod, NULL);
}