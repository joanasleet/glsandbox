#include "Logger.h"
#include "GLHelper.h"

FILE* scrollLog;
FILE* watchLog;

GLenum GLattribIds[] = {
    GL_MAJOR_VERSION,
    GL_MINOR_VERSION,
    GL_MAX_VERTEX_ATTRIBS,
    GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
    GL_MAX_VERTEX_UNIFORM_COMPONENTS,
    GL_ACTIVE_TEXTURE,
    GL_MAX_TEXTURE_SIZE,
    GL_MAX_3D_TEXTURE_SIZE,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE,
    GL_MAX_TEXTURE_IMAGE_UNITS,
    GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
    GL_MAX_COLOR_TEXTURE_SAMPLES,
    GL_MAX_ARRAY_TEXTURE_LAYERS,
    GL_CURRENT_PROGRAM,
    GL_ARRAY_BUFFER_BINDING,
    GL_ELEMENT_ARRAY_BUFFER_BINDING,
    GL_MAX_DRAW_BUFFERS,
    GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
    GL_STEREO,
    GL_MAX_VARYING_FLOATS,
    GL_DEPTH_TEST
};

const char* GLattribStrings[] = {
    "GL_MAJOR_VERSION",
    "GL_MINOR_VERSION",
    "GL_MAX_VERTEX_ATTRIBS",
    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
    "GL_ACTIVE_TEXTURE",
    "GL_MAX_TEXTURE_SIZE",
    "GL_MAX_3D_TEXTURE_SIZE",
    "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
    "GL_MAX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
    "GL_MAX_COLOR_TEXTURE_SAMPLES",
    "GL_MAX_ARRAY_TEXTURE_LAYERS",
    "GL_CURRENT_PROGRAM",
    "GL_ARRAY_BUFFER_BINDING",
    "GL_ELEMENT_ARRAY_BUFFER_BINDING",
    "GL_MAX_DRAW_BUFFERS",
    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
    "GL_STEREO",
    "GL_MAX_VARYING_FLOATS",
    "GL_DEPTH_TEST",
};

void openLog() {
    scrollLog = fopen(SCROLL_LOG_NAME, "a+");
    if (!scrollLog) {
        ERR("Failed to open log <%s>", SCROLL_LOG_NAME);
    }
}

void closeLog() {
    if (scrollLog) {
        fclose(scrollLog);
    }
}

void printWatchLog() {
    watchLog = fopen(WATCH_LOG_NAME, "w");
    if (!watchLog) {
        ERR("Failed to open log <%s>", WATCH_LOG_NAME);
        return;
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