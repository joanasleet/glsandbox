#include "ErrorHandler.h"
#include "GLHelper.h"

void catchError() {
    GLenum error = glGetError();
    switch (error) {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            printError("An unacceptable value is specified for an enumerated"
                    " argument. The offending command is ignored and has no"
                    " other side effect than to set the error flag.");
            break;
        case GL_INVALID_VALUE:
            printError("A numeric argument is out of range."
                    " The offending command is ignored and has no other side"
                    " effect than to set the error flag.");
            break;
        case GL_INVALID_OPERATION:
            printError("The specified operation is not allowed in the current"
                    " state. The offending command is ignored and has no other"
                    " side effect than to set the error flag.");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            printError("The framebuffer object is not complete. The offending"
                    " command is ignored and has no other side effect than to"
                    " set the error flag.");
            break;
        case GL_OUT_OF_MEMORY:
            printError("There is not enough memory left to execute the command."
                    " The state of the GL is undefined, except for the state of"
                    " the error flags, after this error is recorded.");
            break;
        case GL_STACK_UNDERFLOW:
            printError("An attempt has been made to perform an operation that"
                    " would cause an internal stack to underflow.");
            break;
        case GL_STACK_OVERFLOW:
            printError("An attempt has been made to perform an operation that"
                    " would cause an internal stack to overflow.");
            break;
        default:
            printError("Unknown error.");
            fprintf(stderr, "(Error Code: %i)\n", error);
            break;
    }
}

void printError(const char * msg) {

    char* time = getTime();
    fprintf(stderr, "\n[%s] ERROR: %s\n", time, msg);
}

char* getTime() {

    size_t buffLen = 20;
    char* timeStr = (char*) malloc(sizeof(char)*buffLen);
    
    time_t rawtime = time(0);
    strftime(timeStr, buffLen, "%d.%m.%y %H:%M:%S", localtime(&rawtime));

    return timeStr;
}