//
// Created by DexrnZacAttack on 3/27/2025.
//

#include <World/SplitSave.h>

#include <regex>

namespace lce::world {
    SplitSave::SplitSave(int16_t x, int16_t z, int16_t dim): Region(x, z, dim) {
    }

    SplitSave::SplitSave(std::vector<uint8_t> data, int16_t x, int16_t z, int16_t dim): Region(x, z, dim) {
    }

    SplitSave::SplitSave(std::wstring filename) {
    }

    SplitSave::SplitSave(std::vector<uint8_t> data, std::wstring filename, lce::compression::CompressionType outerCompression, ByteOrder endian) {
        std::map<int16_t, int16_t> xz = getXZFromFilename(filename);
        int16_t dim = getDimFromFilename(filename);

        this->dim = dim;

        try {
            this->x = xz.at(0);
            this->z = xz.at(1);
        } catch (const std::out_of_range& e) {
            std::cerr << "Couldn't get the XZ coordinates of the Split Save Region: " << e.what() << std::endl;
            this->x = 0;
            this->z = 0;
        }
    }

    std::map<int16_t, int16_t> SplitSave::getXZFromFilename(const std::wstring& name) {
        std::map<int16_t, int16_t> result;
        std::wregex re(L"(GAMEDATA)_(\\d{4})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

        std::wsmatch match;

        if (std::regex_match(name, match, re)) {
            int x = std::stoi(match.str(3), nullptr, 16);
            int z = std::stoi(match.str(4), nullptr, 16);

            result.insert({1, x});
            result.insert({2, z});
        }

        return result;
    }

    // TODO: verify that DIM is written in hex or decimal
    int16_t SplitSave::getDimFromFilename(std::wstring name) {
        std::wregex re(L"(GAMEDATA)_(\\d{4})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");
        std::wsmatch match;

        if (std::regex_match(name, match, re)) {
            const int dim = std::stoi(match.str(2), nullptr, 16);
            return dim;
        }

        return -9999;
    }
} // lce::world