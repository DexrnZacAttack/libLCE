//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "ColourCommons.h"

namespace lce::colour {
    ColourCommons::ColourCommons() {}

    ColourCommons::ColourCommons(std::string name): name(name) {}

    uint32_t ColourCommons::getSize() {
        return sizeof(uint16_t);
    }
} // lce::colour