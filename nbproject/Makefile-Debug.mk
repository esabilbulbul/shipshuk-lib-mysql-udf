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
CND_PLATFORM=mingw64_2-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/distance.o \
	${OBJECTDIR}/shipshuk_lib.o \
	${OBJECTDIR}/simil.o


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
LDLIBSOPTIONS=-L\"../../../../../../../Program\ Files/MySQL/mysql8/lib\" -L\"../../../../../../../Program\ Files/MySQL/mysql8/lib/plugin\" -L\"../../../../../../../Program\ Files/MySQL/MySQL\ Server\ 5.7/lib\" -L\"../../../../../../../Program\ Files/MySQL/MySQL\ Server\ 5.7/lib/plugin\" -L../../../../../../DB/mysql/lib -L../../../../../../DB/mysql/lib/plugin

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libshipshuk_lib.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libshipshuk_lib.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libshipshuk_lib.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared

${OBJECTDIR}/distance.o: distance.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../../../../../../Program\ Files/MySQL/mysql8/include -I../../../../../../../Program\ Files/MySQL/MySQL\ Server\ 5.7/include -I../../../../../../DB/mysql/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/distance.o distance.c

${OBJECTDIR}/shipshuk_lib.o: shipshuk_lib.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../../../../../../Program\ Files/MySQL/mysql8/include -I../../../../../../../Program\ Files/MySQL/MySQL\ Server\ 5.7/include -I../../../../../../DB/mysql/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/shipshuk_lib.o shipshuk_lib.c

${OBJECTDIR}/simil.o: simil.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../../../../../../Program\ Files/MySQL/mysql8/include -I../../../../../../../Program\ Files/MySQL/MySQL\ Server\ 5.7/include -I../../../../../../DB/mysql/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/simil.o simil.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
