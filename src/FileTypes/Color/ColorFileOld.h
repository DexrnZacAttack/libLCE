//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORFILEOLD_H
#define COLORFILEOLD_H
#include <optional>

#include "Colors/Color.h"
#include "ColorFileCommons.h"

namespace lce::color {

class LIBLCE_API ColorFileOld : public ColorFileCommons {
    public:
    ColorFileOld();
    explicit ColorFileOld(std::vector<Color> colors);
    ColorFileOld(std::vector<Color> colors, uint32_t version);

    static ColorFileOld read(std::vector<uint8_t> data);

    uint8_t *create() const override;
    uint32_t getSize() const override;
};

} // lce::color

#endif //COLORFILEOLD_H
