//
// Created by DexrnZacAttack on 1/2/2025.
//

#include <Color/Color.h>
#include <Color/ColorFile.h>

namespace lce::color {
    Color::Color(std::string name, ARGB color)
        : ColorCommons(name), color(color) {}

    Color Color::read(std::vector<uint8_t> data) {
        io::BinaryIO io((data.data()));
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto color = io.readBE<ARGB>();

        return {name, color};
    }

    // ugh doing this again
    Color Color::read(io::BinaryIO &io) {
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto color = io.readLE<ARGB>();

        return {name, color};
    }

    size_t Color::getSize() const {
        return sizeof(uint16_t) + name.size() + sizeof(ARGB);
    }

    uint8_t *Color::serialize() const {
        io::BinaryIO io(getSize());

        io.writeBE<uint16_t>(name.size());
        io.writeUtf8(name);
        io.writeLE<ARGB>(color);

        return io.getData();
    }

    ColorCommons::ColorCommons() {}

    ColorCommons::ColorCommons(std::string name) : name(name) {}
} // namespace lce::color
