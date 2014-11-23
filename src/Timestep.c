#include "Timestep.h"

#include "common.h"

#ifdef _POSIX_C_SOURCE
#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif
#endif

#include <time.h>

// some static var tracking current time state


void startTimer() {

}

long stopTimer() {

	return 0L;
}

