#ifndef SCRIPT_H
#define	SCRIPT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define ARG_NUMBER 1
#define ARG_STRING 2
#define ARG_TABLE 3

    void initScript(const char* script);

    void argScript(char argType, void* value);

    void exeScript();
    void termScript();

#ifdef	__cplusplus
}
#endif

#endif	/* SCRIPT_H */

