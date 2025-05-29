//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORFILECOMMONS_H
#define COLORFILECOMMONS_H
#include <cstdint>
#include <optional>
#include <vector>

#include <Color/Colors/Color.h>
#include <Filesystem/Filesystem.h>

namespace lce::color {

class LIBLCE_API ColorFileCommons {
public:
    uint32_t version;
    std::vector<Color> colors;
    std::optional<Color> getColorByName(std::string name);

    ColorFileCommons() = default;

    ColorFileCommons(const std::vector<Color> & colors, uint32_t version);

    void addColor(Color color);

    virtual uint8_t *create() const = 0;
    virtual uint64_t getSize() const = 0;
};

} // lce::color

#endif //COLORFILECOMMONS_H
