#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define WATCH_LOG_NAME "watch.log"

extern FILE *watchlog;

#define ERR_LOG_NAME "err.log"
#define INFO_LOG_NAME "info.log"

extern FILE *errlog;
extern FILE *infolog;

#define RVOID

#ifdef NODEBUG
#define debug(target, ...)
#else
#define debug(target, ...) (fprintf(target, ##__VA_ARGS__))
#endif

#define err_str() (strerror(errno))
#define shit_happend() (errno != 0 ? 1 : 0)

/*
 * Prints formatted INFO and ERROR messages. */
#define info(msg, ...) debug(stdout, "[INFO] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define warn(msg, ...) debug(stdout, "[WARN] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define err(msg, ...) debug(stderr, "[ERROR] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/* ******************* GUARDS **************************
 * Guards control code flow by evaluating an expression.
 * If true, code will proceed. Otherwise, an action
 * specific to the guard is performed.
 * *****************************************************/

/*
 * Prints error upon failed expression. */
#define err_guard(exp) do { if(!(exp)) { err("[Guard] Assertion failed: (%s)", #exp); } } while(0)

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
 * Resets system-set errors. */
#define clear_syserr() (errno=0)

/*
 * Checks for a system error and prints its error message. */
#define check_syserr() do { check(!shit_happend(), err_str()); clear_syserr(); } while(0)

/*
 * Prints and calls function */
#define info_call(f) do { info(#f); f; } while(0)


/*
 * Prints formatted info string to the scrolling log. */
#define log_info(msg, ...) \
    do { \
        infolog = fopen(INFO_LOG_NAME, "a+"); \
        break_guard(infolog); \
        debug(infolog, "[INFO] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fclose(infolog); \
    } while (0) \
         
/*
 * Prints formatted info string to the scrolling log. */
#define log_warn(msg, ...) \
    do { \
        errlog = fopen(ERR_LOG_NAME, "a+"); \
        break_guard(errlog); \
        debug(errlog, "[WARN] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fclose(errlog); \
    } while (0) \
         
/*
 * Prints formatted error string to the scrolling log. */
#define log_err(msg, ...) \
    do { \
        errlog = fopen(ERR_LOG_NAME, "a+"); \
        break_guard(errlog); \
        debug(errlog, "[ERROR] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fclose(errlog); \
    } while (0) \
         
/*
 * Prints formatted string to the watch log. */
#define watch(msg, ...) \
    do { \
        watchlog = fopen(WATCH_LOG_NAME, "a+"); \
        break_guard(watchlog); \
        debug(watchlog, msg, ##__VA_ARGS__); \
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
        printf("%s took %f sec.\n", #func, dt); \
    } while(0) \
         
#define PULSE(call, freq) \
    do { \
        static int count = 0; \
        if(count++ == freq) { call; count=0; } \
    } while(0) \
         
#endif


