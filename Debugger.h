#ifndef DEBUGGER_H
#define	DEBUGGER_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef DEBUG
#define debug(target, type, msg, ...) fprintf(target, type " %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define debug(target, type, msg, ...)
#endif

#define flush_errno() (errno == 0 ? "None" : strerror(errno))

#define err()
#define info()
#define assert()

#endif	/* DEBUGGER_H */

