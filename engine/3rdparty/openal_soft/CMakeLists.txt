# CMake build file list for OpenAL

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

IF(POLICY CMP0042)
	CMAKE_POLICY(SET CMP0042 OLD)
ENDIF()

IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

PROJECT(pg_openal C)

DISABLE_WARNINGS()

INCLUDE(CheckFunctionExists)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckTypeSize)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)


OPTION(ALSA    "Check for ALSA backend"                ON)
OPTION(OSS     "Check for OSS backend"                 ON)
OPTION(SOLARIS "Check for Solaris backend"             ON)
OPTION(DSOUND  "Check for DirectSound backend"         ON)
OPTION(WINMM   "Check for Windows Multimedia backend"  ON)
OPTION(PORTAUDIO "Check for PortAudio backend"         ON)

OPTION(DLOPEN  "Check for the dlopen API for loading optional libs"  ON)

OPTION(WERROR  "Treat compile warnings as errors"      ON)

SET(EXAMPLES OFF)


IF(WIN32)
    ADD_DEFINITIONS("-D_WIN32")
ENDIF()

SET(LIB_MAJOR_VERSION "1")
SET(LIB_MINOR_VERSION "7")
SET(LIB_BUILD_VERSION "411")
SET(LIB_VERSION "${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_BUILD_VERSION}")
IF(NOT DEFINED LIB_INSTALL_DIR)
  SET(LIB_INSTALL_DIR "lib")
ENDIF(NOT DEFINED LIB_INSTALL_DIR)


CHECK_TYPE_SIZE("long" SIZEOF_LONG)
CHECK_TYPE_SIZE("long long" SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("unsigned int" SIZEOF_UINT)
CHECK_TYPE_SIZE("void*" SIZEOF_VOIDP)


# Add definitions, compiler switches, etc.
INCLUDE_DIRECTORIES(OpenAL32/Include include "${pg_openal_BINARY_DIR}")

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
ENDIF()
IF(NOT CMAKE_DEBUG_POSTFIX)
    SET(CMAKE_DEBUG_POSTFIX "" CACHE STRING
        "Library postfix for debug builds. Normally left blank."
        FORCE)
ENDIF()

IF(MSVC)
    # ???
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -D_DEBUG")
    SET(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -DNDEBUG")
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
ELSE()
    ADD_DEFINITIONS(-Wall)
    CHECK_C_COMPILER_FLAG(-Wextra HAVE_W_EXTRA)
    IF(HAVE_W_EXTRA)
        ADD_DEFINITIONS(-Wextra)
    ENDIF()

    IF(WERROR)
        ADD_DEFINITIONS(-Werror)
    ENDIF()

    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O2 -funroll-loops -D_DEBUG" CACHE STRING
        "Flags used by the compiler during Release with Debug Info builds."
        FORCE)
    SET(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG" CACHE STRING
        "Flags used by the compiler during release minsize builds."
        FORCE)
    SET(CMAKE_C_FLAGS_RELEASE "-O2 -funroll-loops -fomit-frame-pointer -DNDEBUG" CACHE STRING
        "Flags used by the compiler during release builds"
        FORCE)
    SET(CMAKE_C_FLAGS_DEBUG "-g3 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during debug builds."
        FORCE)

    # Set visibility options if available
    IF(NOT WIN32)
        CHECK_C_SOURCE_COMPILES("int foo() __attribute__((destructor));
                                 int main() {return 0;}" HAVE_GCC_DESTRUCTOR)

        CHECK_C_COMPILER_FLAG(-fvisibility=hidden HAVE_VISIBILITY_SWITCH)
        IF(HAVE_VISIBILITY_SWITCH)
            CHECK_C_SOURCE_COMPILES("int foo() __attribute__((visibility(\"default\")));
                                     int main() {return 0;}" HAVE_GCC_VISIBILITY)
            IF(HAVE_GCC_VISIBILITY)
                ADD_DEFINITIONS(-fvisibility=hidden -DHAVE_GCC_VISIBILITY)
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()

CHECK_INCLUDE_FILE(fenv.h HAVE_FENV_H)
CHECK_INCLUDE_FILE(float.h HAVE_FLOAT_H)

CHECK_LIBRARY_EXISTS(m  sqrtf  "" HAVE_SQRTF)
CHECK_LIBRARY_EXISTS(m  acosf  "" HAVE_ACOSF)
CHECK_LIBRARY_EXISTS(m  atanf  "" HAVE_ATANF)
CHECK_LIBRARY_EXISTS(m  fabsf  "" HAVE_FABSF)
IF(HAVE_FENV_H)
    CHECK_LIBRARY_EXISTS(m  fesetround  "" HAVE_FESETROUND)
ENDIF()
IF(HAVE_SQRTF OR HAVE_ACOSF OR HAVE_ATANF OR HAVE_FABSF OR HAVE_FESETROUND)
    SET(EXTRA_LIBS m ${EXTRA_LIBS})
ENDIF()
CHECK_FUNCTION_EXISTS(strtof HAVE_STRTOF)
CHECK_FUNCTION_EXISTS(_controlfp HAVE__CONTROLFP)

CHECK_FUNCTION_EXISTS(strcasecmp HAVE_STRCASECMP)
IF(NOT HAVE_STRCASECMP)
    CHECK_FUNCTION_EXISTS(_stricmp HAVE__STRICMP)
    IF(NOT HAVE__STRICMP)
        MESSAGE(FATAL_ERROR "No case-insensitive compare function found, please report!")
    ENDIF()

    ADD_DEFINITIONS(-Dstrcasecmp=_stricmp)
ENDIF()

CHECK_FUNCTION_EXISTS(strncasecmp HAVE_STRNCASECMP)
IF(NOT HAVE_STRNCASECMP)
    CHECK_FUNCTION_EXISTS(_strnicmp HAVE__STRNICMP)
    IF(NOT HAVE__STRNICMP)
        MESSAGE(FATAL_ERROR "No case-insensitive size-limitted compare function found, please report!")
    ENDIF()

    ADD_DEFINITIONS(-Dstrncasecmp=_strnicmp)
ENDIF()

#CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
IF(NOT HAVE_SNPRINTF)
    #CHECK_FUNCTION_EXISTS(_snprintf HAVE__SNPRINTF)
    IF(NOT HAVE__SNPRINTF)
        MESSAGE(FATAL_ERROR "No snprintf function found, please report!")
    ENDIF()

    ADD_DEFINITIONS(-Dsnprintf=_snprintf)
ENDIF()

# Check for the dlopen API (for dynamicly loading backend libs)
IF(DLOPEN)
    CHECK_INCLUDE_FILE(dlfcn.h HAVE_DLFCN_H)
    IF(HAVE_DLFCN_H)
        CHECK_LIBRARY_EXISTS(dl dlopen "" HAVE_LIBDL)
        IF(HAVE_LIBDL)
            SET(EXTRA_LIBS dl ${EXTRA_LIBS})
        ENDIF()
    ENDIF()
ENDIF()

# Check if we have Windows headers
CHECK_INCLUDE_FILE(windows.h HAVE_WINDOWS_H -D_WIN32_WINNT=0x0500)
IF(NOT HAVE_WINDOWS_H)
    CHECK_FUNCTION_EXISTS(gettimeofday HAVE_GETTIMEOFDAY)
    IF(NOT HAVE_GETTIMEOFDAY)
        MESSAGE(FATAL_ERROR "No timing function found!")
    ENDIF()

    CHECK_FUNCTION_EXISTS(nanosleep HAVE_NANOSLEEP)
    IF(NOT HAVE_NANOSLEEP)
        MESSAGE(FATAL_ERROR "No sleep function found!")
    ENDIF()

    # We need pthreads outside of Windows
    CHECK_INCLUDE_FILE(pthread.h HAVE_PTHREAD_H)
    IF(NOT HAVE_PTHREAD_H)
        MESSAGE(FATAL_ERROR "PThreads is required for non-Windows builds!")
    ENDIF()

    # Some systems need pthread_np.h to get recursive mutexes
    CHECK_INCLUDE_FILES("pthread.h;pthread_np.h" HAVE_PTHREAD_NP_H)

    # _GNU_SOURCE is needed on some systems for extra attributes, and
    # _REENTRANT is needed for libc thread-safety
    ADD_DEFINITIONS(-D_GNU_SOURCE=1 -D_REENTRANT)
    CHECK_LIBRARY_EXISTS(pthread pthread_create "" HAVE_LIBPTHREAD)
    IF(HAVE_LIBPTHREAD)
        SET(EXTRA_LIBS pthread ${EXTRA_LIBS})
    ENDIF()
ENDIF()

# Check for a 64-bit type
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)
IF(NOT HAVE_STDINT_H)
    IF(HAVE_WINDOWS_H)
        CHECK_C_SOURCE_COMPILES("\#define _WIN32_WINNT 0x0500
                                 \#include <windows.h>
                                 __int64 foo;
                                 int main() {return 0;}" HAVE___INT64)
    ENDIF()
    IF(NOT HAVE___INT64)
        IF(NOT SIZEOF_LONG MATCHES "8")
            IF(NOT SIZEOF_LONG_LONG MATCHES "8")
                MESSAGE(FATAL_ERROR "No 64-bit types found, please report!")
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()


SET(OPENAL_OBJS  OpenAL32/alAuxEffectSlot.c
                 OpenAL32/alBuffer.c
                 OpenAL32/alEffect.c
                 OpenAL32/alError.c
                 OpenAL32/alExtension.c
                 OpenAL32/alFilter.c
                 OpenAL32/alListener.c
                 OpenAL32/alSource.c
                 OpenAL32/alState.c
                 OpenAL32/alThunk.c
)
SET(ALC_OBJS  Alc/ALc.c
              Alc/ALu.c
              Alc/alcConfig.c
              Alc/alcReverb.c
              Alc/alcRing.c
              Alc/alcThread.c
              Alc/bs2b.c
)

SET(BACKENDS "")

# Check ALSA backend
IF(ALSA)
    CHECK_INCLUDE_FILE(alsa/asoundlib.h HAVE_ALSA_ASOUNDLIB_H)
    IF(HAVE_ALSA_ASOUNDLIB_H)
        CHECK_LIBRARY_EXISTS(asound snd_pcm_open "" HAVE_LIBASOUND)
        IF(HAVE_DLFCN_H OR HAVE_LIBASOUND)
            SET(HAVE_ALSA 1)
            SET(ALC_OBJS  ${ALC_OBJS} Alc/alsa.c)
            IF(HAVE_DLFCN_H)
                SET(BACKENDS  "${BACKENDS} ALSA ")
            ELSE()
                SET(BACKENDS  "${BACKENDS} ALSA \(linked\) ")
                SET(EXTRA_LIBS asound ${EXTRA_LIBS})
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()

# Check OSS backend
IF(OSS)
    CHECK_INCLUDE_FILE(sys/soundcard.h HAVE_SYS_SOUNDCARD_H)
    IF(HAVE_SYS_SOUNDCARD_H)
        SET(HAVE_OSS 1)
        SET(ALC_OBJS  ${ALC_OBJS} Alc/oss.c)
        SET(BACKENDS  "${BACKENDS} OSS ")
    ENDIF()
ENDIF()

# Check Solaris backend
IF(SOLARIS)
    CHECK_INCLUDE_FILE(sys/audioio.h HAVE_SYS_AUDIOIO_H)
    IF(HAVE_SYS_AUDIOIO_H)
        SET(HAVE_SOLARIS 1)
        SET(ALC_OBJS  ${ALC_OBJS} Alc/solaris.c)
        SET(BACKENDS  "${BACKENDS} Solaris ")
    ENDIF()
ENDIF()

# Check DSound/MMSystem backend
IF(DSOUND)
    CHECK_INCLUDE_FILE(dsound.h HAVE_DSOUND_H)
    IF(HAVE_DSOUND_H)
        CHECK_LIBRARY_EXISTS(dsound DirectSoundCreate "" HAVE_LIBDSOUND)
        IF(HAVE_LIBDSOUND OR WIN32)
            SET(HAVE_DSOUND 1)
            SET(ALC_OBJS  ${ALC_OBJS} Alc/dsound.c)

            IF(WIN32)
                SET(BACKENDS  "${BACKENDS} DirectSound ")
            ELSE()
                SET(BACKENDS  "${BACKENDS} DirectSound \(linked\) ")
                SET(EXTRA_LIBS dsound ${EXTRA_LIBS})
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()
IF(HAVE_WINDOWS_H)
    IF(WINMM)
        CHECK_INCLUDE_FILES("windows.h;mmsystem.h" HAVE_MMSYSTEM_H -D_WIN32_WINNT=0x0500)
        IF(HAVE_MMSYSTEM_H)
            SET(HAVE_WINMM 1)
            SET(ALC_OBJS  ${ALC_OBJS} Alc/winmm.c)
            SET(BACKENDS  "${BACKENDS} WinMM ")

            SET(CMAKE_REQUIRED_LIBRARIES winmm)
            CHECK_C_SOURCE_COMPILES("int main() {return 0;}" HAVE_LIBWINMM)
            SET(CMAKE_REQUIRED_LIBRARIES "")
#            CHECK_LIBRARY_EXISTS(winmm waveInOpen "" HAVE_LIBWINMM)
            IF(HAVE_LIBWINMM)
                SET(EXTRA_LIBS winmm ${EXTRA_LIBS})
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()

# Check PortAudio backend
IF(PORTAUDIO)
    CHECK_INCLUDE_FILE(portaudio.h HAVE_PORTAUDIO_H)
    IF(HAVE_PORTAUDIO_H)
        CHECK_LIBRARY_EXISTS(portaudio Pa_Initialize "" HAVE_LIBPORTAUDIO)
        IF(HAVE_LIBPORTAUDIO)
            SET(HAVE_PORTAUDIO 1)
            SET(ALC_OBJS  ${ALC_OBJS} Alc/portaudio.c)
            IF(HAVE_DLFCN_H)
                SET(BACKENDS  "${BACKENDS} PortAudio ")
            ELSE()
                SET(BACKENDS  "${BACKENDS} PortAudio \(linked\) ")
                SET(EXTRA_LIBS portaudio ${EXTRA_LIBS})
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()

# This is always available
SET(BACKENDS  "${BACKENDS}")

# Needed for openal.pc.in
SET(prefix ${CMAKE_INSTALL_PREFIX})
SET(exec_prefix "\${prefix}")
SET(libdir "\${exec_prefix}/${LIB_INSTALL_DIR}")
SET(bindir "\${exec_prefix}/bin")
SET(includedir "\${prefix}/include")
SET(PACKAGE_VERSION "${LIB_VERSION}")

# End configuration
CONFIGURE_FILE(
    "${pg_openal_SOURCE_DIR}/config.h.in"
    "${pg_openal_BINARY_DIR}/config.h")

ADD_DEFINITIONS(-DAL_BUILD_LIBRARY)

# Build a library
IF(NOT LIBTYPE)
    SET(LIBTYPE SHARED)
ENDIF()
ADD_LIBRARY(pg_openal SHARED ${LIBTYPE} ${OPENAL_OBJS} ${ALC_OBJS})
#SET_TARGET_PROPERTIES(pg_openal PROPERTIES VERSION ${LIB_VERSION}
#                                            SOVERSION ${LIB_MAJOR_VERSION})

SET_TARGET_PROPERTIES(pg_openal PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

IF(WIN32)
	SET_TARGET_PROPERTIES(pg_openal PROPERTIES PREFIX "")
ENDIF()

TARGET_LINK_LIBRARIES(pg_openal ${EXTRA_LIBS})

# Add an install target here

IF(EXAMPLES)
    ADD_EXECUTABLE(openal-info examples/openal-info.c)
    TARGET_LINK_LIBRARIES(openal-info pg_openal)
    INSTALL(TARGETS openal-info
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION ${LIB_INSTALL_DIR}
            ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    )
ENDIF()

MESSAGE(STATUS "OpenAL backends: ${BACKENDS}")

IF(WIN32)
    IF(NOT HAVE_DSOUND)
        MESSAGE(STATUS "WARNING: Building the Windows version without DirectSound output")
    ENDIF()
ENDIF()
