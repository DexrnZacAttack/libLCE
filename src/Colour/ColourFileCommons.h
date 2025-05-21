//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLOURFILECOMMONS_H
#define COLOURFILECOMMONS_H
#include <cstdint>
#include <optional>
#include <vector>

#include "Colours/Colour.h"
#include "../Filesystem/Filesystem.h"

namespace lce::colour {

class LIBLCE_API ColourFileCommons : public fs::Filesystem {
public:
    uint32_t version;
    std::vector<Colour> colours;
    std::optional<Colour> getColourByName(std::string name);

    ColourFileCommons() = default;

    ColourFileCommons(const std::vector<Colour> & colours, uint32_t version);

    void addColour(Colour colour);
    
    uint64_t getSize() const override;
};

} // lce::colour

#endif //COLOURFILECOMMONS_H
