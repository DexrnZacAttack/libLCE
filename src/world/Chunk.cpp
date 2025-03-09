//
// Created by DexrnZacAttack on 12/28/2024.
//

#include "Chunk.h"
#include "../compression/Chunk.h"

#include <utility>

#include "../compression/Compression.h"
#include "../io/BinaryIO.h"

namespace lce::world {
    CompressedChunk::CompressedChunk() {
    }

    CompressedChunk::CompressedChunk(uint32_t compressedSize, uint32_t uncompressedSize) : uncompressedSize(uncompressedSize) {
        this->compressedSize = compressedSize & 0x3FFFFFFF;
        this->hasRLE = compressedSize & 0x1;
        this->unkFlag = compressedSize & 0x2;
    };

    CompressedChunk::CompressedChunk(uint32_t compressedSize, uint32_t uncompressedSize, bool hasRLE, bool unkFlag) : compressedSize(compressedSize), uncompressedSize(uncompressedSize), hasRLE(hasRLE), unkFlag(unkFlag) {}

    CompressedChunk::CompressedChunk(std::vector<uint8_t> data, ByteOrder endian) {
        io::BinaryIO io(data.data(), data.size());

        const auto compressedSizeFlags = io.read<uint32_t>(endian);
        this->compressedSize = compressedSizeFlags & 0x3FFFFFFF;
        this->hasRLE = compressedSizeFlags & 0x1;
        this->unkFlag = compressedSizeFlags & 0x2;

        this->uncompressedSize = io.read<uint32_t>(endian);
        this->data = io.readOfSizeVec(this->compressedSize);
    }

    CompressedChunk::CompressedChunk(uint8_t* data, ByteOrder endian) {
        io::BinaryIO io(data);

        const auto compressedSizeFlags = io.read<uint32_t>(endian);
        this->compressedSize = compressedSizeFlags & 0x3FFFFFFF;
        this->hasRLE = compressedSizeFlags & 0x1;
        this->unkFlag = compressedSizeFlags & 0x2;

        this->uncompressedSize = io.read<uint32_t>(endian);
        this->data = io.readOfSizeVec(this->compressedSize);
    }

    std::vector<uint8_t> CompressedChunk::decompress(compression::CompressionType compressionType) {
        std::vector<uint8_t> outerData = decompressOuter(compressionType);
        std::vector<uint8_t> innerData = decompressInner(outerData);
        return innerData;
    };

    std::vector<uint8_t> CompressedChunk::decompressInner(std::vector<uint8_t> data) {
        return compression::Chunk::decompress(std::move(data));
    };

    std::vector<uint8_t> CompressedChunk::decompressOuter(compression::CompressionType compressionType) {
        // todo
        throw std::exception("Not implemented");
    };
} // lce::world