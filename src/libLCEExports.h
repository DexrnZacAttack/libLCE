//
// Created by DexrnZacAttack on 4/10/2025.
//

#ifndef LIBLCEEXPORTS_H
#define LIBLCEEXPORTS_H
#include "libLCE.h"

// prevents duplicating these defines over and over

#define VERSION "1.0.8"
#define LIBRARY_STRING "libLCE v" VERSION " (" COMPILER_NAME " | " PLATFORM_NAME " " PLATFORM_ARCH ") | https://github.com/DexrnZacAttack/libLCE"

LIBLCE_API NO_DISCARD const char* getLibraryVersion();
LIBLCE_API NO_DISCARD void printLibraryInfo();

#endif //LIBLCEEXPORTS_H
