//
// Created by DexrnZacAttack on 1/2/2025.
//

#include <Color/Color.h>

namespace lce::color {
    WorldColor::WorldColor(std::string name, ARGB waterColor,
                           ARGB underwaterColor, ARGB fogColor)
        : ColorCommons(name), waterColor(waterColor),
          underwaterColor(underwaterColor), fogColor(fogColor) {}

    WorldColor WorldColor::read(std::vector<uint8_t> data) {
        io::BinaryIO io((data.data()));
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto water = io.readBE<ARGB>();
        auto underwater = io.readBE<ARGB>();
        auto fog = io.readBE<ARGB>();

        return {name, water, underwater, fog};
    }

    WorldColor WorldColor::read(io::BinaryIO &io) {
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto water = io.readLE<ARGB>();
        auto underwater = io.readLE<ARGB>();
        auto fog = io.readLE<ARGB>();

        return {name, water, underwater, fog};
    }

    uint8_t *WorldColor::serialize() const {
        io::BinaryIO io(getSize());

        io.writeBE<uint16_t>(name.size());
        io.writeUtf8(name);
        io.writeLE<ARGB>(waterColor);
        io.writeLE<ARGB>(underwaterColor);
        io.writeLE<ARGB>(fogColor);

        return io.getData();
    }

    size_t WorldColor::getSize() const {
        return sizeof(uint16_t) + name.size() + sizeof(ARGB) + sizeof(ARGB) +
               sizeof(ARGB);
    }
} // namespace lce::color
