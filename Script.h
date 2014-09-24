#ifndef SCRIPT_H
#define	SCRIPT_H

#include <stdlib.h>

#define ARG_NUMBER 1
#define ARG_STRING 2
#define ARG_TABLE 3

void initScript(const char* script);

void argScript(char argType, void* value);

void exeScript();
void dumpResult();

double popNumber();
const char* popString();

void termScript();

#endif	/* SCRIPT_H */

