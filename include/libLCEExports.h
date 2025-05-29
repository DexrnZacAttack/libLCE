//
// Created by DexrnZacAttack on 4/10/2025.
//

#ifndef LIBLCEEXPORTS_H
#define LIBLCEEXPORTS_H
#include <libLCE.h>

// prevents duplicating these defines over and over

#if CMAKE_BUILD_DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif

#define VERSION "1.1.1"
#define LIBRARY_STRING                                                                                                 \
    "libLCE v" VERSION " (" COMPILER_NAME " / " BUILD_TYPE " | " PLATFORM_NAME " " PLATFORM_ARCH                       \
    ") | https://github.com/DexrnZacAttack/libLCE"

LIBLCE_API NO_DISCARD const char* getLibraryVersion();
LIBLCE_API NO_DISCARD void printLibraryInfo();

#endif // LIBLCEEXPORTS_H
