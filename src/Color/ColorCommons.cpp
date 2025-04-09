//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "ColorCommons.h"

namespace lce::color {
    ColorCommons::ColorCommons() {}

    ColorCommons::ColorCommons(std::string name): name(name) {}

    uint32_t ColorCommons::getSize() {
        return sizeof(uint16_t);
    }
} // lce::color