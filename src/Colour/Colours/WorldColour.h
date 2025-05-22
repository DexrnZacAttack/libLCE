//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef WORLDCOLOUR_H
#define WORLDCOLOUR_H
#include "Colour.h"
#include "ColourCommons.h"

namespace lce::colour {

class LIBLCE_API WorldColour : public ColourCommons {
    public:
    ARGB waterColour;
    ARGB underwaterColour;
    ARGB fogColour;

    WorldColour(std::string name, ARGB waterColour, ARGB underwaterColour, ARGB fogColour);

    static WorldColour read(std::vector<uint8_t> data);
    static WorldColour read(io::BinaryIO &io);

    uint8_t *create();
    uint32_t getSize() override;
};

} // lce::colour

#endif //WORLDCOLOUR_H
