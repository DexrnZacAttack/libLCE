//
// Created by DexrnZacAttack on 1/2/2025.
//

#include <Color/ColorFile.h>
#include <IO/BinaryIO.h>

namespace lce::color {
    ColorFile::ColorFile() {}

    ColorFile::ColorFile(std::vector<Color> colors,
                         std::vector<WorldColor> worldColors)
        : ColorFileCommons(colors, 4), worldColors(worldColors) {}

    ColorFile ColorFile::read(std::vector<uint8_t> data) {
        lce::io::BinaryIO io(data.data());
        ColorFile cfo;

        cfo.version = io.readBE<uint32_t>();
        auto colorCount = io.readBE<uint32_t>();

        for (uint32_t i = 0; i < colorCount; i++) {
            cfo.colors.push_back(Color::read(io));
        }

        auto worldColorCount = io.readBE<uint32_t>();

        for (uint32_t i = 0; i < worldColorCount; i++) {
            cfo.worldColors.push_back(WorldColor::read(io));
        }

        return cfo;
    }

    uint64_t ColorFile::getSize() const {
        uint32_t size = 12; // 4 for version 4 for count 4 for biome count
        for (auto color : colors) {
            size += 2;
            size += (color.name).size();
            size += 4;
        }
        for (auto color : worldColors) {
            size += 2;
            size += (color.name).size();
            size += 4; // water
            size += 4; // underwater
            size += 4; // fog
        }
        return size;
    }

    uint8_t *ColorFile::serialize() const {
        io::BinaryIO io(this->getSize());

        io.writeBE<uint32_t>(this->version);
        io.writeBE<uint32_t>(this->colors.size());

        for (auto color : colors) {
            io.writeBytes(color.serialize(), color.getSize());
        }

        io.writeBE<uint32_t>(this->worldColors.size());

        for (auto color : worldColors) {
            io.writeBytes(color.serialize(), color.getSize());
        }

        return io.getData();
    }

    std::optional<Color> ColorFileCommons::getColorByName(std::string name) {
        const auto find = std::find_if(
            colors.begin(), colors.end(),
            [&name](const Color &color) { return color.name == name; });

        if (find != colors.end())
            return *find;

        return std::nullopt;
    }

    ColorFileCommons::ColorFileCommons(const std::vector<Color> &colors,
                                       uint32_t version)
        : colors(colors), version(version) {}
} // namespace lce::color
