//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef WORLDCOLOR_H
#define WORLDCOLOR_H
#include "Color.h"
#include "ColorCommons.h"

namespace lce::color {

class WorldColor : public ColorCommons {
    public:
    ARGB waterColor;
    ARGB underwaterColor;
    ARGB fogColor;

    WorldColor(std::string name, ARGB waterColor, ARGB underwaterColor, ARGB fogColor);

    static WorldColor read(std::vector<uint8_t> data);
    static WorldColor read(io::BinaryIO &io);

    uint8_t *create();
    uint32_t getSize() override;
};

} // lce::color

#endif //WORLDCOLOR_H
