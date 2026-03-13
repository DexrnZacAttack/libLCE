//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "LCE/color/ColorFile.h"
#include <BinaryIO/buffer/BinaryBuffer.h>

namespace lce::color {
    ColorFile::ColorFile(bio::buffer::BinaryBuffer &io) {
        this->mVersion = io.readBE<uint32_t>();

        const uint32_t colorCount = io.readBE<uint32_t>();
        for (uint32_t i = 0; i < colorCount; i++) {
            const uint16_t l = io.readBE<uint16_t>();
            std::string name = io.readString(l);

            this->mColors.emplace(name, Color(io));
        }

        const uint32_t worldColorCount = io.readBE<uint32_t>();
        for (uint32_t i = 0; i < worldColorCount; i++) {
            const uint16_t l = io.readBE<uint16_t>();
            std::string name = io.readString(l);

            this->mWorldColors.emplace(name, WorldColor(io));
        }
    }

    void ColorFile::addWorldColor(const std::string &name, WorldColor color) {
        this->mWorldColors.emplace(std::move(name), std::move(color));
    }

    WorldColor *ColorFile::getWorldColor(const std::string &name) {
        if (const auto c = this->mWorldColors.find(name);
            c != this->mWorldColors.end())
            return &(c->second);

        return nullptr;
    }

    std::unordered_map<std::string, WorldColor> &ColorFile::getWorldColors() {
        return this->mWorldColors;
    }

    size_t ColorFile::getSize() const {
        uint32_t size = sizeof(uint32_t); // version

        size += sizeof(uint32_t); // color count
        for (auto [name, color] : this->mColors) {
            size += sizeof(uint16_t);
            size += name.size();
            size += color.getSize();
        }

        size += sizeof(uint32_t); // world color count
        for (auto [name, color] : this->mWorldColors) {
            size += sizeof(uint16_t);
            size += name.size();
            size += color.getSize();
        }
        return size;
    }

    uint8_t *ColorFile::serialize() const {
        bio::buffer::BinaryBuffer io(this->getSize());

        io.writeBE<uint32_t>(this->mVersion);
        io.writeBE<uint32_t>(this->mColors.size());

        for (auto [name, color] : this->mColors) {
            io.writeBE<uint16_t>(name.size());
            io.writeString(name, false);

            io.writeBytes(color.serialize(), color.getSize());
        }

        io.writeBE<uint32_t>(this->mWorldColors.size());

        for (auto [name, color] : this->mWorldColors) {
            io.writeBE<uint16_t>(name.size());
            io.writeString(name, false);

            io.writeBytes(color.serialize(), color.getSize());
        }

        return io.begin();
    }

    Color *ColorFileCommons::getColor(const std::string &name) {
        if (const auto c = this->mColors.find(name); c != this->mColors.end())
            return &(c->second);

        return nullptr;
    }

    std::unordered_map<std::string, Color> &ColorFileCommons::getColors() {
        return this->mColors;
    }

    ColorFileCommons::ColorFileCommons(
        const std::unordered_map<std::string, Color> &colors,
        const uint32_t version)
        : mVersion(version), mColors(colors) {}

    ColorFileCommons *ColorFileCommons::deserializeAuto(uint8_t *data) {
        bio::buffer::BinaryBuffer io(data);

        const ColorFileVersion version = io.readBE<ColorFileVersion>();

        if (version == OLD)
            return new ColorFileOld(data);

        return new ColorFile(data);
    }

    void ColorFileCommons::addColor(const std::string &name, Color color) {
        this->mColors.emplace(std::move(name), std::move(color));
    }

    uint32_t ColorFileCommons::getVersion() const { return this->mVersion; }
} // namespace lce::color
