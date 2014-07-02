#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ErrorHandler.o \
	${OBJECTDIR}/GLCamera.o \
	${OBJECTDIR}/GLContext.o \
	${OBJECTDIR}/GLHelper.o \
	${OBJECTDIR}/GLObject.o \
	${OBJECTDIR}/GLProgram.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/stb_image.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs glew` `pkg-config --libs glfw3`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/glsandbox

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/glsandbox: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/glsandbox ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/ErrorHandler.o: ErrorHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall `pkg-config --cflags glew` `pkg-config --cflags glfw3`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ErrorHandler.o ErrorHandler.cpp

${OBJECTDIR}/GLCamera.o: GLCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall `pkg-config --cflags glew` `pkg-config --cflags glfw3`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GLCamera.o GLCamera.cpp

${OBJECTDIR}/GLContext.o: GLContext.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall `pkg-config --cflags glew` `pkg-config --cflags glfw3`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GLContext.o GLContext.cpp

${OBJECTDIR}/GLHelper.o: GLHelper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall `pkg-config --cflags glew` `pkg-config --cflags glfw3`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GLHelper.o GLHelper.cpp

${OBJECTDIR}/GLObject.o: GLObject.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall `pkg-config --cflags glew` `pkg-config --cflags glfw3`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GLObject.o GLObject.cpp

${OBJECTDIR}/GLProgram.o: GLProgram.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall `pkg-config --cflags glew` `pkg-config --cflags glfw3`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GLProgram.o GLProgram.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall `pkg-config --cflags glew` `pkg-config --cflags glfw3`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/stb_image.o: stb_image.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g `pkg-config --cflags glew` `pkg-config --cflags glfw3`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stb_image.o stb_image.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/glsandbox

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
