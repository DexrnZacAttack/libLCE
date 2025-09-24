//
// Created by DexrnZacAttack on 1/2/2025.
//

#include <Color/ColorFile.h>
#include <IO/BinaryIO.h>

namespace lce::color {
    ColorFileOld::ColorFileOld(
        const std::unordered_map<std::string, Color> &colors)
        : ColorFileCommons(colors, 0) {}

    ColorFileOld::ColorFileOld(
        const std::unordered_map<std::string, Color> &colors,
        const uint32_t version)
        : ColorFileCommons(colors, version) {}

    ColorFileOld::ColorFileOld(io::BinaryIO &io) {
        this->version = io.readBE<uint32_t>();

        const uint32_t cc = io.readBE<uint32_t>();
        for (uint32_t i = 0; i < cc; i++) {
            const uint16_t l = io.readBE<uint16_t>();
            std::string name = io.readString(l);

            this->colors.emplace(name, Color(io));
        }
    }

    uint8_t *ColorFileOld::serialize() const {
        io::BinaryIO io(this->getSize());

        io.writeBE<uint32_t>(this->version);

        io.writeBE<uint32_t>(this->colors.size());
        for (auto [name, color] : colors) {
            io.writeBE<uint16_t>(name.size());
            io.writeString(name);

            io.writeBytes(color.serialize(), color.getSize());
        }

        return io.getData();
    }

    size_t ColorFileOld::getSize() const {
        uint32_t size = sizeof(uint32_t); // version

        size += sizeof(uint32_t); // color count
        for (auto [name, color] : colors) {
            size += sizeof(uint16_t);
            size += name.size();
            size += color.getSize();
        }

        return size;
    }
} // namespace lce::color
