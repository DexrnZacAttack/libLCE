//
// Created by DexrnZacAttack on 4/10/2025.
//
#include "LCE/libLCEExports.h"

namespace lce {
    extern "C" {
    // clang-format off
        TO_SECTION(".liblce.dexrn.me") constexpr const char *lce_get_build_type() { return LIBLCE_BUILD_TYPE; } // NOLINT
        TO_SECTION(".liblce.dexrn.me") constexpr const char *lce_get_compiler_name() { return LIBLCE_COMPILER_NAME; } // NOLINT
        TO_SECTION(".liblce.dexrn.me") constexpr const char *lce_get_platform_arch() { return LIBLCE_PLATFORM_ARCH; } // NOLINT
        TO_SECTION(".liblce.dexrn.me") constexpr const char *lce_get_platform() { return LIBLCE_PLATFORM_NAME; } // NOLINT
        TO_SECTION(".liblce.dexrn.me") constexpr const char *lce_get_version() { return LIBLCE_VERSION; } // NOLINT
        TO_SECTION(".liblce.dexrn.me") constexpr const char *lce_get_library_string() { return lce::LIBRARY_STRING; } // NOLINT
    // clang-format on
    }
} // namespace lce