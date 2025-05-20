//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "ColourFile.h"

#include <fstream>

#include "../IO/BinaryIO.h"

namespace lce::colour {
    ColourFile::ColourFile() {}

    ColourFile::ColourFile(std::vector<Colour> colours, std::vector<WorldColour> worldColours): ColourFileCommons(colours, 4), worldColours(worldColours) {}

    ColourFile ColourFile::read(std::vector<uint8_t> data) {
        lce::io::BinaryIO io(data.data());
        ColourFile cfo;

        cfo.version = io.readBE<uint32_t>();
        auto colourCount = io.readBE<uint32_t>();

        for (uint32_t i = 0; i < colourCount; i++) {
            cfo.colours.push_back(Colour::read(io));
        }

        auto worldColourCount = io.readBE<uint32_t>();

        for (uint32_t i = 0; i < worldColourCount; i++) {
            cfo.worldColours.push_back(WorldColour::read(io));
        }

        return cfo;
    }

    uint32_t ColourFile::getSize() const {
        uint32_t size = 12; // 4 for version 4 for count 4 for biome count
        for (auto colour : colours) {
            size += 2;
            size += (colour.name).size();
            size += 4;
        }
        for (auto colour : worldColours) {
            size += 2;
            size += (colour.name).size();
            size += 4; // water
            size += 4; // underwater
            size += 4; // fog
        }
        return size;
    }

    uint8_t *ColourFile::create() const {
        io::BinaryIO io(this->getSize());

        io.writeBE<uint32_t>(this->version);
        io.writeBE<uint32_t>(this->colours.size());

        for (auto colour : colours) {
            io.writeBytes(colour.create(), colour.getSize());
        }

        io.writeBE<uint32_t>(this->worldColours.size());

        for (auto colour : worldColours) {
            io.writeBytes(colour.create(), colour.getSize());
        }

        return io.getData();
    }
} // lce
