#include "Timestep.h"

#include "common.h"

#ifdef _POSIX_C_SOURCE
#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif
#endif

#include <time.h>

static double start, current, elapsed;
static double globalSynchedTime = 0.0;

void startTimer() {
    start = globalSynchedTime;
}

double elapsedTime() {

    updateGlobalTime();

    current = globalSynchedTime;
    elapsed = current - start;
    start = current;

    return elapsed;
}

double getGlobalTime() {
    return globalSynchedTime;
}

void updateGlobalTime() {
    globalSynchedTime = glfwGetTime();
}


void wait(long nanosec) {

    // undefined in c99
    // sleepPeriod.tv_sec = 0;
    // sleepPeriod.tv_nsec = nanosec;

    // nanosleep(&sleepPeriod, NULL);
}