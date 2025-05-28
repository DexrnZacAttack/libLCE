//
// Created by DexrnZacAttack on 12/28/2024.
//

#ifndef REGION_H
#define REGION_H

#include <map>
#include <vector>
#include <string>
#include <array>

#include "../libLCE.h"
#include "../Compression/Compression.h"
#include "../IO/BinaryIO.h"

namespace lce::world {
    class Chunk;

    struct LIBLCE_API ChunkLocation {
        int24_t offset;
        uint32_t size;
    };

    struct LIBLCE_API RegionChunk {
        ChunkLocation location;
        uint32_t timestamp;
        lce::world::Chunk* chunk;
    };

    class LIBLCE_API Region {
    protected:
        /// X Coordinate
        int16_t x;
        /// Z Coordinate
        int16_t z;
        /// Dimension
        int16_t dim;
    public:
        std::array<RegionChunk, 1024> chunks;

        //region constructors
        Region() = default;
        Region(int16_t x, int16_t z, int16_t dim);
        Region(std::vector<uint8_t> data, int16_t x, int16_t z, int16_t dim, compression::CompressionType outerCompression = lce::compression::CompressionType::ZLIB, ByteOrder endian = LITTLE);
        explicit Region(std::wstring filename);
        Region(std::vector<uint8_t> data, std::wstring filename, lce::compression::CompressionType outerCompression = lce::compression::CompressionType::ZLIB, ByteOrder endian = LITTLE);
        //endregion

        //region helpers
        static std::map<int16_t, int16_t> getXZFromFilename(const std::wstring& name);
        static int16_t getDimFromFilename(std::wstring name);
        //endregion

        //region get/set
        [[nodiscard]] int16_t getX() const;
        [[nodiscard]] int16_t getZ() const;
        [[nodiscard]] int16_t getDim() const;
        void setX(int16_t x);
        void setZ(int16_t z);
        void setDim(int16_t dim);

        int16_t getSizeInSectors() const;
        //endregion
    };
} // lce::world

#endif //REGION_H
