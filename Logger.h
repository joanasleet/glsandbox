#ifndef LOGGER_H
#define	LOGGER_H

#include "common.h"

#include <ctime>
#include <cstdarg>

#define LOG_NAME "gl_watch.log"
extern FILE* watchlog;

#define INFO(args, ...) \
do { \
    char* time = getTime(); \
    fprintf(stdout, "[%s] INFO: " args "\n", time, ##__VA_ARGS__); \
    free(time); \
} while (0) \

#define ERR(args, ...) \
do { \
    char* time = getTime(); \
    fprintf(stderr, "[%s][%s:%i] ERROR: " args "\n", time, __FILE__, __LINE__, ##__VA_ARGS__); \
    free(time); \
} while (0) \

#define FLOG(args, ...) \
do { \
    char* time = getTime(); \
    fprintf(watchlog, "[%s][%s:%i] LOG: " args "\n", time, __FILE__, __LINE__, ##__VA_ARGS__); \
    free(time); \
} while (0) \

// init log file
void initLog();

// exception catching
void catchError();

// helper 
char* getTime();

#endif	/* ERRORHANDLER_H */

