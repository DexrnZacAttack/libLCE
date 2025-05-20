//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORFILECOMMONS_H
#define COLORFILECOMMONS_H
#include <cstdint>
#include <optional>
#include <vector>

#include "Colors/Color.h"

namespace lce::color {

class LIBLCE_API ColorFileCommons {
public:
    uint32_t version;
    std::vector<Color> colors;
    std::optional<Color> getColorByName(std::string name);

    ColorFileCommons() = default;

    ColorFileCommons(const std::vector<Color> & colors, uint32_t version);

    void addColor(Color color);
    virtual uint32_t getSize();
};

} // lce::color

#endif //COLORFILECOMMONS_H
