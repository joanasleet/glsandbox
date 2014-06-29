/* 
 * File:   ErrorHandler.h
 * Author: alex
 *
 * Created on June 27, 2014, 2:29 AM
 */

#ifndef ERRORHANDLER_H
#define	ERRORHANDLER_H

#include <cstdlib>
#include <stdio.h>

#include <ctime>

#include <GL/glew.h>

void catchError();
void printError(const char* msg);
char* getTime();

#endif	/* ERRORHANDLER_H */

