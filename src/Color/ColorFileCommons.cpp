//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "ColorFileCommons.h"
#include <algorithm>

namespace lce::color {
    std::optional<Color> ColorFileCommons::getColorByName(std::string name) {
        const auto find = std::find_if(colors.begin(), colors.end(), [&name](const Color& color) {
            return color.name == name;
        });

        if (find != colors.end()) return *find;

        return std::nullopt;
    }

    ColorFileCommons::ColorFileCommons(const std::vector<Color> &colors, uint32_t version): colors(colors), version(version) {}

    uint32_t ColorFileCommons::getSize() {
        return 4; // atleast 4
    }
} // lce::color