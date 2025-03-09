//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "ColorFileOld.h"

#include "ColorFile.h"

#include <optional>

#include "../io/BinaryIO.h"

namespace lce::color {
    ColorFileOld::ColorFileOld() {}

    ColorFileOld::ColorFileOld(std::vector<Color> colors): ColorFileCommons(colors, 0) {}

    ColorFileOld::ColorFileOld(std::vector<Color> colors, uint32_t version): ColorFileCommons(colors, version) {}

    ColorFileOld ColorFileOld::read(std::vector<uint8_t> data) {
        lce::io::BinaryIO io(data.data());
        ColorFileOld cfo;

        cfo.version = io.readBE<uint32_t>();
        const auto colorCount = io.readBE<uint32_t>();

        uint8_t* datRel = io.getDataRelative();
        for (uint32_t i = 0; i < colorCount; i++) {
            cfo.colors.push_back(Color::read(std::vector(datRel, datRel + data.size() - 4)));
        }

        return cfo;
    }

    uint8_t *ColorFileOld::create() {
        io::BinaryIO io(this->getSize());

        io.writeBE<uint32_t>(this->version);
        io.writeBE<uint32_t>(this->colors.size());

        for (auto color : colors) {
            io.writeBytes(color.create(), color.getSize());
        }

        return io.getData();
    }

    uint32_t ColorFileOld::getSize() {
        uint32_t size = 8; // 4 for version 4 for count
        for (const auto& color : colors) {
            size += 2;
            size += (color.name).size();
            size += 4;
        }
        return size;
    }
} // lce::color