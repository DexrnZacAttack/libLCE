//
// Created by DexrnZacAttack on 3/27/2025.
//

#ifndef SPLITSAVE_H
#define SPLITSAVE_H
#include "Region.h"

namespace lce::world {

class SplitSave : public Region {
    //split save region constructors
    SplitSave() = default;
    SplitSave(int16_t x, int16_t z, int16_t dim);
    SplitSave(std::vector<uint8_t> data, int16_t x, int16_t z, int16_t dim);
    SplitSave(std::wstring filename);
    SplitSave(std::vector<uint8_t> data, std::wstring filename, lce::compression::CompressionType outerCompression = lce::compression::CompressionType::ZLIB, ByteOrder endian = LITTLE);
    //endregion

    //region helpers
    static std::map<int16_t, int16_t> getXZFromFilename(const std::wstring& name);
    static int16_t getDimFromFilename(std::wstring name);
    //endregion

};

} // lce::world

#endif //SPLITSAVE_H
