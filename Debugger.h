#ifndef DEBUGGER_H
#define	DEBUGGER_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NODEBUG
#define debug(target, type, msg, ...)
#else
#define debug(target, type, msg, ...) fprintf(target, type " %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define shit_happend() (errno != 0 ? 1 : 0)
#define err_str() (strerror(errno))

#define err(msg, ...) debug(stderr, "[ERROR]", msg, ##__VA_ARGS__)
#define info(msg, ...) debug(stdout, "[INFO]", msg, ##__VA_ARGS__)
#define check(exp, msg) do { if(!exp) { err("Assertion failed: %s", msg); } } while(0);
#define checkerr() do { if(shit_happend()) { err("%s", err_str()); } } while(0);

#endif	/* DEBUGGER_H */

