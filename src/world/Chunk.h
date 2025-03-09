//
// Created by DexrnZacAttack on 12/28/2024.
//

#ifndef CHUNK_H
#define CHUNK_H
#include <cstdint>
#include <vector>
#include "../io/BinaryIO.h"
#include "../lce.h"

#include "../compression/compression.h"

namespace lce::world {
    class Chunk {
    };
    class CompressedChunk {
    public:
        uint32_t compressedSize;
        uint32_t uncompressedSize;
        bool hasRLE;
        bool unkFlag;
        std::vector<uint8_t> data;

        CompressedChunk();

        CompressedChunk(uint32_t compressedSize, uint32_t uncompressedSize);

        CompressedChunk(uint32_t compressedSize, uint32_t uncompressedSize, bool hasRLE, bool unkFlag);

        CompressedChunk(std::vector<uint8_t> data, ByteOrder endian = ByteOrder::LITTLE);

        CompressedChunk(uint8_t *data, ByteOrder endian);

        std::vector<uint8_t> decompress(compression::CompressionType compressionType);

        static std::vector<uint8_t> decompressInner(std::vector<uint8_t> data);

        std::vector<uint8_t> decompressOuter(compression::CompressionType compressionType);
    };
} // lce::world

#endif //CHUNK_H
