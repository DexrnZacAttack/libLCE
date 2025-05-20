//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "Colour.h"

#include <fstream>

namespace lce::colour {
    Colour::Colour(std::string name, ARGB colour): ColourCommons(name), colour(colour) {}

    Colour Colour::read(std::vector<uint8_t> data) {
        io::BinaryIO io((data.data()));
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto colour = io.readBE<ARGB>();

        return {name, colour};
    }

    // ugh doing this again
    Colour Colour::read(io::BinaryIO& io) {
        const auto strLength = io.readBE<uint16_t>();
        std::string name = io.readUtf8(strLength);

        auto colour = io.readLE<ARGB>();

        return {name, colour};
    }

    uint32_t Colour::getSize() {
        uint32_t size = 0;
        size += sizeof(uint16_t);
        size += name.size();
        size += sizeof(ARGB);
        return size;
    }

    uint8_t *Colour::create() {
        io::BinaryIO io(getSize());

        io.writeBE<uint16_t>(name.size());
        io.writeUtf8(name);
        io.writeLE<ARGB>(colour);

        return io.getData();
    }
} // lce::colour