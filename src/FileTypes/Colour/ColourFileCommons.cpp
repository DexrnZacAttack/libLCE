//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "ColourFileCommons.h"
#include <algorithm>


namespace lce::colour {
    std::optional<Colour> ColourFileCommons::getColourByName(std::string name) {
        const auto find = std::find_if(colours.begin(), colours.end(), [&name](const Colour& colour) {
            return colour.name == name;
        });

        if (find != colours.end()) return *find;

        return std::nullopt;
    }

    ColourFileCommons::ColourFileCommons(const std::vector<Colour> &colours, uint32_t version): colours(colours), version(version) {}

    uint32_t ColourFileCommons::getSize() const {
        return 4; // atleast 4
    }
} // lce::colour
