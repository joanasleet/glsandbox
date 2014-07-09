#ifndef LOGGER_H
#define	LOGGER_H

#include "common.h"

#include <ctime>
#include <cstdarg>

#define LOG_NAME "gl_watch.log"

#define LOG_TO_FILE

#ifdef LOG_TO_FILE
#define ERR_LOG watchlog
#define INFO_LOG watchlog
#else
#define ERR_LOG stderr
#define INFO_LOG stdout
#endif

extern FILE* watchlog;

#define INFO(args, ...) \
do { \
    char* time = getTime(); \
    openLog(); \
    fprintf(INFO_LOG, "[%s] INFO: " args "\n", time, ##__VA_ARGS__); \
    closeLog(); \
    free(time); \
} while (0) \

#define ERR(args, ...) \
do { \
    char* time = getTime(); \
    openLog(); \
    fprintf(ERR_LOG, "[%s][%s:%i] ERROR: " args "\n", time, __FILE__, __LINE__, ##__VA_ARGS__); \
    closeLog(); \
    free(time); \
} while (0) \

#define CATCH_EXCEPTION

// init log file
void openLog();
void closeLog();

// exception catching
void catchError();

// helper 
char* getTime();

#endif	/* ERRORHANDLER_H */

