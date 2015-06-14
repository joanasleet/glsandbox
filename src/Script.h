#ifndef SCRIPT_H
#define SCRIPT_H

#include "common.h"

#define ARG_NUMBER 1
#define ARG_STRING 2
#define ARG_TABLE 3

void execScript(const char *script);
void argScript(uint8 argType, void *value);

int32 popInt();
double popFloat();
double popNumber();
const char *popString();

void dumpResult();
void termScript();

#endif  /* SCRIPT_H */


