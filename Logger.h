#ifndef LOGGER_H
#define	LOGGER_H

#include "common.h"
#include "Camera.h"
#include "ShaderCache.h"

#define TIME_FORMAT_REDC "%H:%M:%S"
#define TIME_FORMAT_FULL "%d.%m.%y %H:%M:%S"

#define SCROLL_LOG_NAME "gl_scroll.log"
#define WATCH_LOG_NAME "gl_watch.log"
#define CACHE_LOG_NAME "gl_cache.log"

#ifdef LOG_TO_FILE
#define ERR_LOG scrollLog
#define INFO_LOG scrollLog
#else
#define ERR_LOG stderr
#define INFO_LOG stdout
#endif

extern FILE* scrollLog;
extern FILE* watchLog;
extern FILE* cacheLog;


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


#define CATCH_EXCEPTION \
do { \
    char* time = getTime(); \
    openLog(); \
    fprintf(ERR_LOG, "[%s][%s:%i] EXCEPTION: %s\n", time, __FILE__, __LINE__, getErrorMessage()); \
    closeLog(); \
    free(time); \
} while (0) \

// GL runtime infos
extern GLenum GLattribIds[];
extern const char* GLattribStrings[];

void printWatchLog(Camera* c);
void printCacheLog(Hash* cache, const char* name = "Cache");

// init log file
void openLog();
void closeLog();

void resetLogs();

// exception catching
const char* getErrorMessage();
void debugCB(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar *msg, void* userParam);

// helper 
char* getTime();

#endif	/* ERRORHANDLER_H */

