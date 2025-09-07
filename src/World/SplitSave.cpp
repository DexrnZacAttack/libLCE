//
// Created by DexrnZacAttack on 3/27/2025.
//

#include <World/SplitSave.h>

#include <regex>

namespace lce::world {
    SplitSave::SplitSave(const int16_t x, const int16_t z, const int16_t dim)
        : Region(x, z, dim) {}

    SplitSave::SplitSave(const std::vector<uint8_t> &data, const int16_t x,
                         const int16_t z, const int16_t dim)
        : Region(x, z, dim) {}

    SplitSave::SplitSave(const std::wstring &filename) {}

    SplitSave::SplitSave(const std::vector<uint8_t> &data,
                         const std::wstring &filename,
                         lce::compression::CompressionType outerCompression,
                         io::ByteOrder byteOrder) {
        const std::map<int16_t, int16_t> xz = getXZFromFilename(filename);
        const int16_t dim = getDimFromFilename(filename);

        this->dim = dim;

        try {
            this->x = xz.at(0);
            this->z = xz.at(1);
        } catch (const std::out_of_range &e) {
            throw std::runtime_error(
                std::string("Couldn't get the XZ coordinates of the Split Save "
                            "Region: ") +
                e.what());
        }
    }

    std::map<int16_t, int16_t>
    SplitSave::getXZFromFilename(const std::wstring &name) {
        std::map<int16_t, int16_t> result;
        const std::wregex re(
            L"(GAMEDATA)_(\\d{4})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

        if (std::wsmatch match; std::regex_match(name, match, re)) {
            int x = std::stoi(match.str(3), nullptr, 16);
            int z = std::stoi(match.str(4), nullptr, 16);

            result.insert({1, x});
            result.insert({2, z});
        }

        return result;
    }

    // TODO: verify that DIM is written in hex or decimal
    int16_t SplitSave::getDimFromFilename(const std::wstring &name) {
        const std::wregex re(
            L"(GAMEDATA)_(\\d{4})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");
        std::wsmatch match;

        if (std::regex_match(name, match, re))
            return std::stoi(match.str(2), nullptr, 16);

        return -9999;
    }
} // namespace lce::world
