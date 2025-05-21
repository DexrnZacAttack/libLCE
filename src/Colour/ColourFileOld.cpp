//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "ColourFileOld.h"

#include "ColourFile.h"

#include <optional>

#include "../IO/BinaryIO.h"

namespace lce::colour {
    ColourFileOld::ColourFileOld() {}

    ColourFileOld::ColourFileOld(std::vector<Colour> colours): ColourFileCommons(colours, 0) {}

    ColourFileOld::ColourFileOld(std::vector<Colour> colours, uint32_t version): ColourFileCommons(colours, version) {}

    ColourFileOld ColourFileOld::read(std::vector<uint8_t> data) {
        lce::io::BinaryIO io(data.data());
        ColourFileOld cfo;

        cfo.version = io.readBE<uint32_t>();
        const auto colourCount = io.readBE<uint32_t>();

        uint8_t* datRel = io.getDataRelative();
        for (uint32_t i = 0; i < colourCount; i++) {
            cfo.colours.push_back(Colour::read(std::vector(datRel, datRel + data.size() - 4)));
        }

        return cfo;
    }

    uint8_t *ColourFileOld::create() const {
        io::BinaryIO io(this->getSize());

        io.writeBE<uint32_t>(this->version);
        io.writeBE<uint32_t>(this->colours.size());

        for (auto colour : colours) {
            io.writeBytes(colour.create(), colour.getSize());
        }

        return io.getData();
    }

    uint64_t ColourFileOld::getSize() const {
        uint32_t size = 8; // 4 for version 4 for count
        for (const auto& colour : colours) {
            size += 2;
            size += (colour.name).size();
            size += 4;
        }
        return size;
    }
} // lce::colour
