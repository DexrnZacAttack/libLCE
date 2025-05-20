//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "WorldColour.h"

namespace lce::colour {
    WorldColour::WorldColour(std::string name, ARGB waterColour, ARGB underwaterColour, ARGB fogColour): ColourCommons(name), waterColour(waterColour), underwaterColour(underwaterColour), fogColour(fogColour) {}

    WorldColour WorldColour::read(std::vector<uint8_t> data) {
        io::BinaryIO io((data.data()));
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto water = io.readBE<ARGB>();
        auto underwater = io.readBE<ARGB>();
        auto fog = io.readBE<ARGB>();

        return {name, water, underwater, fog};
    }

    WorldColour WorldColour::read(io::BinaryIO& io) {
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto water = io.readLE<ARGB>();
        auto underwater = io.readLE<ARGB>();
        auto fog = io.readLE<ARGB>();

        return {name, water, underwater, fog};
    }

    uint8_t *WorldColour::create() {
        io::BinaryIO io(getSize());

        io.writeBE<uint16_t>(name.size());
        io.writeUtf8(name);
        io.writeLE<ARGB>(waterColour);
        io.writeLE<ARGB>(underwaterColour);
        io.writeLE<ARGB>(fogColour);

        return io.getData();
    }

    uint32_t WorldColour::getSize() {
        uint32_t size = 0;
        size += sizeof(uint16_t);
        size += name.size();
        size += sizeof(ARGB);
        size += sizeof(ARGB);
        size += sizeof(ARGB);
        return size;
    }
} // lce::colour