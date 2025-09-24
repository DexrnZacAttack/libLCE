//
// Created by DexrnZacAttack on 1/2/2025.
//

#include <Color/ColorFile.h>
#include <IO/BinaryIO.h>

namespace lce::color {
    ColorFile::ColorFile(io::BinaryIO &io) {
        this->version = io.readBE<uint32_t>();

        const uint32_t colorCount = io.readBE<uint32_t>();
        for (uint32_t i = 0; i < colorCount; i++) {
            const uint16_t l = io.readBE<uint16_t>();
            std::string name = io.readString(l);

            this->colors.emplace(name, Color(io));
        }

        const uint32_t worldColorCount = io.readBE<uint32_t>();
        for (uint32_t i = 0; i < worldColorCount; i++) {
            const uint16_t l = io.readBE<uint16_t>();
            std::string name = io.readString(l);

            this->worldColors.emplace(name, WorldColor(io));
        }
    }

    void ColorFile::addWorldColor(const std::string &name, WorldColor color) {
        this->worldColors.emplace(std::move(name), std::move(color));
    }

    WorldColor *ColorFile::getWorldColor(const std::string &name) {
        if (const auto c = worldColors.find(name); c != worldColors.end())
            return &(c->second);

        return nullptr;
    }

    std::unordered_map<std::string, WorldColor> &ColorFile::getWorldColors() {
        return this->worldColors;
    }

    size_t ColorFile::getSize() const {
        uint32_t size = sizeof(uint32_t); // version

        size += sizeof(uint32_t); // color count
        for (auto [name, color] : colors) {
            size += sizeof(uint16_t);
            size += name.size();
            size += color.getSize();
        }

        size += sizeof(uint32_t); // world color count
        for (auto [name, color] : worldColors) {
            size += sizeof(uint16_t);
            size += name.size();
            size += color.getSize();
        }
        return size;
    }

    uint8_t *ColorFile::serialize() const {
        io::BinaryIO io(this->getSize());

        io.writeBE<uint32_t>(this->version);
        io.writeBE<uint32_t>(this->colors.size());

        for (auto [name, color] : colors) {
            io.writeBE<uint16_t>(name.size());
            io.writeString(name);

            io.writeBytes(color.serialize(), color.getSize());
        }

        io.writeBE<uint32_t>(this->worldColors.size());

        for (auto [name, color] : worldColors) {
            io.writeBE<uint16_t>(name.size());
            io.writeString(name);

            io.writeBytes(color.serialize(), color.getSize());
        }

        return io.getData();
    }

    Color *ColorFileCommons::getColor(const std::string &name) {
        if (const auto c = colors.find(name); c != colors.end())
            return &(c->second);

        return nullptr;
    }

    std::unordered_map<std::string, Color> &ColorFileCommons::getColors() {
        return this->colors;
    }

    ColorFileCommons::ColorFileCommons(
        const std::unordered_map<std::string, Color> &colors,
        const uint32_t version)
        : version(version), colors(colors) {}

    ColorFileCommons *ColorFileCommons::deserializeAuto(uint8_t *data) {
        io::BinaryIO io(data);

        const ColorFileVersion version = io.readBE<ColorFileVersion>();

        if (version == OLD)
            return new ColorFileOld(data);

        return new ColorFile(data);
    }

    void ColorFileCommons::addColor(const std::string &name, Color color) {
        this->colors.emplace(std::move(name), std::move(color));
    }

    uint32_t ColorFileCommons::getVersion() const { return this->version; }
} // namespace lce::color
