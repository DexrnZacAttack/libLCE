//
// Created by DexrnZacAttack on 12/28/2024.
//

#ifndef REGION_H
#define REGION_H
#include <vector>

#include "Chunk.h"

namespace lce::world {
    struct ChunkLocation {
        int24_t offset; // just a typedef, can't be bothered to make actual datatype
        int8_t size;
    };

    struct RegionChunk {
        ChunkLocation location;
        uint32_t timestamp;
        Chunk chunk;
    };

    class Region {
    protected:
        bool isSplitSave;
        compression::CompressionType compressionType = compression::ZLIB;
    public:
        std::vector<RegionChunk> chunks;

        Region();
        explicit Region(bool isSplitSave = false, compression::CompressionType compressionType = compression::ZLIB);
    };
} // lce::world

#endif //REGION_H
