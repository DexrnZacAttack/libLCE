//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "LCE/color/ColorFile.h"
#include <BinaryIO/buffer/BinaryBuffer.h>

namespace lce::color {
    ColorFileOld::ColorFileOld(
        const std::unordered_map<std::string, Color> &colors)
        : ColorFileCommons(colors, 0) {}

    ColorFileOld::ColorFileOld(
        const std::unordered_map<std::string, Color> &colors,
        const uint32_t version)
        : ColorFileCommons(colors, version) {}

    ColorFileOld::ColorFileOld(bio::buffer::BinaryBuffer &io) {
        this->mVersion = io.readBE<uint32_t>();

        const uint32_t cc = io.readBE<uint32_t>();
        for (uint32_t i = 0; i < cc; i++) {
            const uint16_t l = io.readBE<uint16_t>();
            std::string name = io.readString(l);

            this->mColors.emplace(name, Color(io));
        }
    }

    uint8_t *ColorFileOld::serialize() const {
        bio::buffer::BinaryBuffer io(this->getSize());

        io.writeBE<uint32_t>(this->mVersion);

        io.writeBE<uint32_t>(this->mColors.size());
        for (auto [name, color] : this->mColors) {
            io.writeBE<uint16_t>(name.size());
            io.writeString(name, false);

            io.writeBytes(color.serialize(), color.getSize());
        }

        return io.begin();
    }

    size_t ColorFileOld::getSize() const {
        uint32_t size = sizeof(uint32_t); // version

        size += sizeof(uint32_t); // color count
        for (auto [name, color] : this->mColors) {
            size += sizeof(uint16_t);
            size += name.size();
            size += color.getSize();
        }

        return size;
    }
} // namespace lce::color
