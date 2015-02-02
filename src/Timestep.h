#ifndef TIMESTEP_H
#define TIMESTEP_H

#define MILLI_SEC 1000000L

void startTimer();
double elapsedTime();
double getGlobalTime();
void updateGlobalTime();
void pauseGlobalTime();

void wait(long nanosec);

#endif

