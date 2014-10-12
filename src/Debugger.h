#ifndef DEBUGGER_H
#define	DEBUGGER_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define SCROLL_LOG_NAME "scroll.log"
#define WATCH_LOG_NAME "watch.log"

extern FILE* scrolllog;
extern FILE* watchlog;

#ifdef NODEBUG
#define debug(target, frmt, ...)
#else
#define debug(target, frmt, ...) fprintf(target, frmt, ##__VA_ARGS__)
#endif

#define err_str() (strerror(errno))
#define shit_happend() (errno != 0 ? 1 : 0)

/*
 * Prints formatted INFO and ERROR messages. */
#define info(msg, ...) debug(stdout, "[INFO] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define err(msg, ...) debug(stderr, "[ERROR] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/*
 * Checks for true expression and prints error if false. */
#define check(exp, hint) do { if(!(exp)) { err("Check failed: (%s) %s", #exp, hint); } } while(0)

/* ******************* GUARDS **************************
 * Guards control code flow by evaluating an expression.
 * If true, code will proceed. Otherwise, an action
 * specific to the guard is performed.
 * *************************************************** */

/*
 * Prints error upon failed expression. */
#define err_guard(exp) do { if(!(exp)) { err("[Guard] Assertion failed: (%s)", #exp); } } while(0)

/*
 * Exits upon failed expression. */
#define exit_guard(exp) do { if(!(exp)) { err("[Guard] Assertion failed: (%s)", #exp); exit(EXIT_FAILURE); } } while(0)

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
 * Prints formatted info string to the scrolling log. */
#define log_info(msg, ...) \
do { \
    scrolllog = fopen(SCROLL_LOG_NAME, "a+"); \
    check(scrolllog, "Failed to open " SCROLL_LOG_NAME); \
    info(msg, ##__VA_ARGS__); \
    fclose(scrolllog); \
} while (0) \

/*
 * Prints formatted error string to the scrolling log. */
#define log_err(msg, ...) \
do { \
    scrolllog = fopen(SCROLL_LOG_NAME, "a+"); \
    check(scrolllog, "Failed to open " SCROLL_LOG_NAME); \
    err(msg, ##__VA_ARGS__); \
    fclose(scrolllog); \
} while (0) \

/*
 * Prints formatted string to the watch log. */
#define watch(msg, ...) \
do { \
    watchlog = fopen(WATCH_LOG_NAME, "a+"); \
    check(watchlog, "Failed to open " WATCH_LOG_NAME); \
    debug(watchlog, msg, ##__VA_ARGS__); \
    fclose(watchlog); \
} while(0) \

/*
 * Clears the scrolling and- watch log. */
#define clear_logs() \
do { \
    scrolllog = fopen(SCROLL_LOG_NAME, "w"); \
    check(scrolllog, "Failed to open " SCROLL_LOG_NAME); \
    fclose(scrolllog); \
    watchlog = fopen(WATCH_LOG_NAME, "w"); \
    check(watchlog, "Failed to open " WATCH_LOG_NAME); \
    fclose(watchlog); \
} while(0) \

#endif	/* DEBUGGER_H */

