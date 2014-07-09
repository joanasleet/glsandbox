#include "Logger.h"
#include "GLHelper.h"

FILE* watchlog;

void openLog() {
    watchlog = fopen(LOG_NAME, "a+");
    if (!watchlog) {
        ERR("Failed to open log <%s>", LOG_NAME);
    }
}

void closeLog() {
    if (watchlog) {
        fclose(watchlog);
    }
}

void catchError() {
    GLenum error = glGetError();
    switch (error) {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            ERR("An unacceptable value is specified for an enumerated argument."
                    " The offending command is ignored and has no other side"
                    " effect than to set the error flag. (Code: %i)", error);
            break;
        case GL_INVALID_VALUE:
            ERR("A numeric argument is out of range."
                    " The offending command is ignored and has no other side"
                    " effect than to set the error flag. (Code: %i)", error);
            break;
        case GL_INVALID_OPERATION:
            ERR("The specified operation is not allowed in the current"
                    " state. The offending command is ignored and has no other"
                    " side effect than to set the error flag. (Code: %i)", error);
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            ERR("The framebuffer object is not complete. The offending"
                    " command is ignored and has no other side effect than to"
                    " set the error flag. (Code: %i)", error);
            break;
        case GL_OUT_OF_MEMORY:
            ERR("There is not enough memory left to execute the command."
                    " The state of the GL is undefined, except for the state of"
                    " the error flags, after this error is recorded. (Code: %i)", error);
            break;
        case GL_STACK_UNDERFLOW:
            ERR("An attempt has been made to perform an operation that"
                    " would cause an internal stack to underflow. (Code: %i)", error);
            break;
        case GL_STACK_OVERFLOW:
            ERR("An attempt has been made to perform an operation that"
                    " would cause an internal stack to overflow. (Code: %i)", error);
            break;
        default:
            ERR("Unknown error (Code: %i)", error);
            break;
    }
}

char* getTime() {

    size_t buffLen = 20;
    char* timeStr = (char*) malloc(sizeof (char)*buffLen);

    time_t rawtime = time(0);
    strftime(timeStr, buffLen, "%d.%m.%y %H:%M:%S", localtime(&rawtime));

    return timeStr;
}