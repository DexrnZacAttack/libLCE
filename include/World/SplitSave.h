//
// Created by DexrnZacAttack on 3/27/2025.
//

#ifndef SPLITSAVE_H
#define SPLITSAVE_H
#include <World/Region.h>

namespace lce::world {

    class SplitSave : public Region {
      public:
        // split save region constructors
        SplitSave() = default;
        SplitSave(int16_t x, int16_t z, int16_t dim);
        SplitSave(const std::vector<uint8_t> &data, int16_t x, int16_t z,
                  int16_t dim);
        SplitSave(const std::wstring &filename);
        SplitSave(const std::vector<uint8_t> &data,
                  const std::wstring &filename,
                  lce::compression::CompressionType outerCompression =
                      lce::compression::CompressionType::ZLIB,
                  io::ByteOrder byteOrder = io::ByteOrder::LITTLE);
        // endregion

        // region helpers
        static std::map<int16_t, int16_t>
        getXZFromFilename(const std::wstring &name);
        static int16_t getDimFromFilename(const std::wstring &name);
        // endregion
    };

} // namespace lce::world

#endif // SPLITSAVE_H
