//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef WORLDCOLOR_H
#define WORLDCOLOR_H
#include <Color/Colors/Color.h>
#include <Color/Colors/ColorCommons.h>

namespace lce::color {

class LIBLCE_API WorldColor : public ColorCommons {
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
