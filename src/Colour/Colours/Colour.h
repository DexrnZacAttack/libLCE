//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLOUR_H
#define COLOUR_H
#include <string>
#include <vector>

#include "ColourCommons.h"
#include "../../IO/BinaryIO.h"

namespace lce::colour {

struct ARGB {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class LIBLCE_API Colour : public ColourCommons {
    public:
    ARGB colour;

    Colour(std::string name, ARGB colour);
    static Colour read(std::vector<uint8_t> data);

    static Colour read(io::BinaryIO &io);
    uint8_t *create();
    uint32_t getSize() override;
};

} // lce::colour

#endif //COLOUR_H
