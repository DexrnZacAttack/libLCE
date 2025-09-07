//
// Created by DexrnZacAttack on 4/10/2025.
//
#include <libLCEExports.h>

namespace lce {
    extern "C" {
    const char *getLibraryVersion() { return LIBLCE_VERSION; }

    void printLibraryInfo() { std::cout << LIBRARY_STRING << std::endl; }
    }
} // namespace lce