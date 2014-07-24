#include "Logger.h"

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

const char* SEVERITY_LVL[] = {
    "HIGH",
    "MEDIUM",
    "LOW"
};

const char* SOURCE[] = {
    "API",
    "WINDOW SYSTEM",
    "SHADER COMPILER",
    "THIRD PARTY",
    "APPLICATION",
    "OTHER"
};

const char* TYPE[] = {
    "ERROR",
    "DEPRECATED BEHAVIOUR",
    "UNDEFINED DEHAVIOUR",
    "PORTABILITY",
    "PERFORMANCE",
    "OTHER"
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

void printWatchLog(Camera* cam) {
    watchLog = fopen(WATCH_LOG_NAME, "w");
    if (!watchLog) {
        ERR("Failed to open log <%s>", WATCH_LOG_NAME);
        return;
    }

    fprintf(watchLog, "–––––––––––––– Watch Log ––––––––––––––\n");
    fprintf(watchLog, "Cam direction:\t(%.1f, %.1f, %.1f)\n", cam->dirX, cam->dirY, cam->dirZ);
    fprintf(watchLog, "Cam position:\t(%.1f, %.1f, %.1f)\n", cam->xPos, cam->yPos, cam->zPos);

    fclose(watchLog);
}

const char* getErrorMessage() {

    switch (glGetError()) {
        case GL_NO_ERROR:
            return "None";
            break;
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
            break;
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
            break;
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
            break;
        default:
            return "Unknown shit happend";
            break;
    }
    return "";
}

char* getTime() {

    char* timeStr = (char*) malloc(sizeof (char)*20);
    time_t rawtime = time(0);
    strftime(timeStr, 20, TIME_FORMAT_REDC, localtime(&rawtime));

    return timeStr;
}

void debugCB(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar *msg, void* userParam) {

    int src_i = source - 0x8246;
    int type_i = type - 0x824C;
    int sev_i = severity - 0x9146;

    ERR("\n\tSource: %s %s\n\tId: %u\n\tSeverity: %s\n\tUserParam: %i\n\tMessage:\n%s",
            SOURCE[src_i], TYPE[type_i], id, SEVERITY_LVL[sev_i], *(int*) userParam,
            msg);
}