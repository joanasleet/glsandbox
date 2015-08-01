#ifndef UTIL_H
#define UTIL_H

/* * * * * * * * * * + *
 * Debugging utilities *
 * * * * * * + * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define WATCH_LOG_NAME "logs/watch.log"
#define ERR_LOG_NAME "logs/err.log"
#define INFO_LOG_NAME "logs/info.log"

extern FILE *errlog;
extern FILE *infolog;
extern FILE *watchlog;

#define RVOID

#ifdef NODEBUG
#define debug(target, ...)
#else
#define debug(target, ...) (fprintf(target, __VA_ARGS__))
#endif

/*
 * Prints formatted messages. */
#define info(msg, ...) debug(stdout, "[INFO]  %s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__)
#define warn(msg, ...) debug(stdout, "[WARN]  %s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__)
#define err(msg, ...)  debug(stderr, "[ERROR] %s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__)

/*
 * Prints error upon failed expression. */
#define err_guard(exp) do { if(!(exp)) { err("[Guard] Assertion failed: (%s)", #exp); } } while(0)
#define warn_guard(exp) do { if(!(exp)) { warn("[Guard] Assertion failed: (%s)", #exp); } } while(0)
/*
 * Exits upon failed expression. */
#define exit_guard(exp) do { if(!(exp)) { err("[Guard] Assertion failed: (%s)", #exp); exit(EXIT_FAILURE); } } while(0)

/*
 * Breaks upon failed expression. */
#define break_guard(exp) do { if(!(exp)) { err("[Guard] Assertion failed: (%s)", #exp); break; } } while(0)

/*
 * Returns upon failed expression. */
#define return_guard(exp, ...) do { if(!(exp)) { err("[Guard] Assertion failed: (%s)", #exp); return __VA_ARGS__ ; } } while(0)

/*
 * Checks for a system error and prints its error message. */
#define check_syserr() do { if(errno!=0) err("[System] %s (%d)", strerror(errno), errno); errno=0; } while(0)

/*
 * Prints and calls function */
#define info_call(f) do { info("%s", #f); f; } while(0)

/*
 * Prints formatted info string to the scrolling log. */
#define log_info(msg, ...) \
    do { \
        infolog = fopen(INFO_LOG_NAME, "a+"); \
        break_guard(infolog); \
        debug(infolog, "[INFO]  %s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); \
        fclose(infolog); \
    } while (0) \
         
/*
 * Prints formatted info string to the scrolling log. */
#define log_warn(msg, ...) \
    do { \
        errlog = fopen(ERR_LOG_NAME, "a+"); \
        break_guard(errlog); \
        debug(errlog, "[WARN]  %s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); \
        fclose(errlog); \
    } while (0) \
         
/*
 * Prints formatted error string to the scrolling log. */
#define log_err(msg, ...) \
    do { \
        errlog = fopen(ERR_LOG_NAME, "a+"); \
        break_guard(errlog); \
        debug(errlog, "[ERROR] %s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); \
        fclose(errlog); \
    } while (0) \
         
/*
 * Prints formatted string to the watch log. */
#define watch(msg, ...) \
    do { \
        watchlog = fopen(WATCH_LOG_NAME, "a+"); \
        break_guard(watchlog); \
        debug(watchlog, msg, __VA_ARGS__); \
        fclose(watchlog); \
    } while(0) \
         
/*
 * Clears the scrolling and- watch log. */
#define clear_logs() \
    do { \
        infolog = fopen(INFO_LOG_NAME, "w"); \
        err_guard(infolog); \
        fclose(infolog); \
        errlog = fopen(ERR_LOG_NAME, "w"); \
        err_guard(errlog); \
        fclose(errlog); \
        watchlog = fopen(WATCH_LOG_NAME, "w"); \
        err_guard(watchlog); \
        fclose(watchlog); \
    } while(0) \
         
/*
 * Measures execution time of func */
#define MEASURE(func) \
    do { \
        clock_t start, end; \
        double dt; \
        start = clock(); \
        func; \
        end = clock(); \
        dt = (double)(end-start) / CLOCKS_PER_SEC; \
        info("%s took %f sec.", #func, dt); \
    } while(0) \
         
/*
 * Executes call an a given frequency */
#define PULSE(call, freq) \
    do { \
        static int count = 0; \
        if(count++ == freq) { call; count=0; } \
    } while(0) \
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * *
 * allocation utilities  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define alloc(type, n) ((type*)malloc( sizeof(type) * (size_t)n ))
#define ralloc(ptr, type, n) (realloc(ptr, sizeof(type) * (size_t)n)

/* * * * * * * * * * * * * * *
 * data structures utilities *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define DefList(type)                                               \
typedef struct {                                                    \
    int len;                                                        \
    int cap;                                                        \
    type* data;                                                     \
} type##List;                                                       \
                                                                    \
static type##List* type##List_new(int cap) {                        \
    type##List* list = alloc(type##List, 1);                        \
    err_guard(list);                                                \
    list->data = alloc(type, cap);                                  \
    err_guard(list->data);                                          \
    list->len = 0;                                                  \
    list->cap = cap;                                                \
    return list;                                                    \
}                                                                   \
                                                                    \
static type type##List_get(int i, type##List* list) {               \
    err_guard(i <= (list->len-1));                                  \
    return list->data[i];                                           \
}                                                                   \
                                                                    \
static void type##List_set(int i, type val, type##List* list) {     \
    return_guard(i <= (list->len-1), RVOID);                        \
    list->data[i] = val;                                            \
}                                                                   \
                                                                    \
static void type##List_add(type val, type##List* list) {            \
    if(list->len == list->cap) {                                    \
        list->cap *= 2;                                             \
        list->data = ralloc(list->data, type, list->cap));          \
        err("Raising cap to %d.", list->cap);                       \
    }                                                               \
    list->data[(list->len)++] = val;                                \
}                                                                   \
                                                                    \
static void type##List_free(type##List* list) {                     \
    free(list->data);                                               \
    free(list);                                                     \
}                                                                   \
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */        

#endif

