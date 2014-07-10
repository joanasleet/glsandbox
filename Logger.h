#ifndef LOGGER_H
#define	LOGGER_H

#include "common.h"

#define SCROLL_LOG_NAME "gl_scroll.log"
#define WATCH_LOG_NAME "gl_watch.log"

#ifdef LOG_TO_FILE
#define ERR_LOG scrollLog
#define INFO_LOG scrollLog
#else
#define ERR_LOG stderr
#define INFO_LOG stdout
#endif

extern FILE* scrollLog;
extern FILE* watchLog;

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

// GL runtime infos
extern GLenum GLattribIds[];
extern const char* GLattribStrings[];

// init log file
void openLog();
void closeLog();

// print watchable log
void printWatchLog();

// exception catching
void catchError();

// helper 
char* getTime();

#endif	/* ERRORHANDLER_H */

