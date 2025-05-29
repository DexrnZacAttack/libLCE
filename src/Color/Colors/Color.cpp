//
// Created by DexrnZacAttack on 1/2/2025.
//

#include <Color/Colors/Color.h>
#include <Color/Colors/ColorCommons.h>

#include <fstream>

namespace lce::color {
    Color::Color(std::string name, ARGB color): color::ColorCommons(name), color(color) {}

    Color Color::read(std::vector<uint8_t> data) {
        io::BinaryIO io((data.data()));
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto color = io.readBE<ARGB>();

        return {name, color};
    }

    Color Color::read(io::BinaryIO& io) {
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto color = io.readLE<ARGB>();

        return {name, color};
    }

    uint32_t Color::getSize() {
        uint32_t size = 0;
        size += sizeof(uint16_t);
        size += name.size();
        size += sizeof(ARGB);
        return size;
    }

    uint8_t *Color::create() {
        io::BinaryIO io(getSize());

        io.writeBE<uint16_t>(name.size());
        io.writeUtf8(name);
        io.writeLE<ARGB>(color);

        return io.getData();
    }
} // lce::color
