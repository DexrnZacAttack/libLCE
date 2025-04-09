//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORFILE_H
#define COLORFILE_H
#include "ColorFileCommons.h"
#include "WorldColor.h"

namespace lce::color {

class LIBLCE_API ColorFile : public ColorFileCommons {
    public:
    std::vector<WorldColor> worldColors;

    ColorFile();
    ColorFile(std::vector<Color> colors, std::vector<WorldColor> worldColors);

    static ColorFile read(std::vector<uint8_t> data);
    uint32_t getSize() override;

    uint8_t *create();
};

} // lce::color

#endif //COLORFILE_H
