//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORFILE_H
#define COLORFILE_H
#include <Color/ColorFileCommons.h>
#include <Color/Colors/WorldColor.h>

namespace lce::color {

class LIBLCE_API ColorFile : public ColorFileCommons {
    public:
    std::vector<WorldColor> worldColors;

    ColorFile();
    ColorFile(std::vector<Color> colors, std::vector<WorldColor> worldColors);

    static ColorFile read(std::vector<uint8_t> data);
    uint64_t getSize() const;

    uint8_t *create() const;
};

} // lce::color

#endif //COLORFILE_H
