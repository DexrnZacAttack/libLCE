//
// Created by DexrnZacAttack on 4/10/2025.
//

#ifndef LIBLCEEXPORTS_H
#define LIBLCEEXPORTS_H
#include "libLCE.h"

// prevents duplicating these defines over and over

#if CMAKE_BUILD_DEBUG
#define LIBLCE_BUILD_TYPE "Debug"
#else
#define LIBLCE_BUILD_TYPE "Release"
#endif

namespace lce {
    TO_SECTION(".data.liblce.dexrn.me")
    constexpr const char *const LIBRARY_STRING =
        "libLCE v" LIBLCE_VERSION " (" LIBLCE_COMPILER_NAME
        " / " LIBLCE_BUILD_TYPE " | " LIBLCE_PLATFORM_NAME
        " " LIBLCE_PLATFORM_ARCH ") | https://github.com/DexrnZacAttack/libLCE";

    // clang-format off
    extern "C" {
        TO_SECTION(".liblce.dexrn.me") LIBLCE_API NO_DISCARD constexpr const char *lce_get_build_type(); // NOLINT
        TO_SECTION(".liblce.dexrn.me") LIBLCE_API NO_DISCARD constexpr const char *lce_get_compiler_name(); // NOLINT
        TO_SECTION(".liblce.dexrn.me") LIBLCE_API NO_DISCARD constexpr const char *lce_get_platform_arch(); // NOLINT
        TO_SECTION(".liblce.dexrn.me") LIBLCE_API NO_DISCARD constexpr const char *lce_get_platform(); // NOLINT
        TO_SECTION(".liblce.dexrn.me") LIBLCE_API NO_DISCARD constexpr const char *lce_get_version(); // NOLINT
        TO_SECTION(".liblce.dexrn.me") LIBLCE_API NO_DISCARD constexpr const char *lce_get_library_string(); // NOLINT
    }
    // clang-format on
} // namespace lce

#endif // LIBLCEEXPORTS_H
