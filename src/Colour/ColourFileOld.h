//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLOURFILEOLD_H
#define COLOURFILEOLD_H
#include <optional>

#include "Colours/Colour.h"
#include "ColourFileCommons.h"

namespace lce::colour {

class LIBLCE_API ColourFileOld : public ColourFileCommons {
    public:
    ColourFileOld();
    explicit ColourFileOld(std::vector<Colour> colours);
    ColourFileOld(std::vector<Colour> colours, uint32_t version);

    static ColourFileOld read(std::vector<uint8_t> data);

    uint8_t *create() const override;
    uint64_t getSize() const override;
};

} // lce::colour

#endif //COLOURFILEOLD_H
