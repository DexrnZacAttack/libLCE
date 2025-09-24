//
// Created by DexrnZacAttack on 12/28/2024.
//

#include <Compression/Compression.h>
#include <IO/BinaryIO.h>
#include <World/Chunk.h>

#include <fstream>
#include <sstream>
#include <utility>

namespace lce::world {
    Chunk::Section::Section() {}

    Chunk::Chunk(std::vector<uint8_t> data,
                 compression::Compression::Type outerCompression,
                 io::ByteOrder byteOrder) {
        io::BinaryIO io(data.data());

        const auto compressedSizeFlags = io.read<uint32_t>(byteOrder);
        const uint32_t compressedSize = compressedSizeFlags & 0x3FFFFFFF;
        bool hasRle = compressedSizeFlags & 0x1;
        bool unk = compressedSizeFlags & 0x2;

        const uint32_t uncompressedSize = io.read<uint32_t>(byteOrder);
        // ugh we have to get another size if we're on PS3

        DebugLog("hasRle: " << hasRle);
        DebugLog("uncompressedSize: " << uncompressedSize);
        DebugLog("compressedSize: " << compressedSize);

        /// unmodified compressed chunk data
        std::vector<uint8_t> compData = io.readOfSizeVec(compressedSize);
        /// data from first layer decompression, note that the size given does
        /// not fully reflect the decompressed size of the first layer.
        std::vector<uint8_t> uncompData(uncompressedSize);

        const bool err = compression::Compression::decompressZlibWithLength(
            compData, uncompData, uncompressedSize, 0);
        if (err)
            throw std::runtime_error("Outer chunk decompression failed");

        std::vector<uint8_t> chunkData(uncompressedSize);

        compression::Compression::decompressChunk(uncompData, chunkData);

        // if (hasRle) {
        //     chunkData =
        //     std::make_unique<std::vector<uint8_t>>(uncompressedSize);
        //     compression::Compression::decompressChunk(uncompData,
        //     *chunkData);
        // } else {
        //     chunkData =
        //     std::make_unique<std::vector<uint8_t>>(uncompData.begin(),
        //     uncompData.end());
        // }

        io::BinaryIO chunkIO(chunkData.data());

        // uint16_t ver = chunkIO.readBE<uint16_t>();
        // int32_t x = chunkIO.readBE<int32_t>();
        // int32_t z = chunkIO.readBE<int32_t>();

        // std::ostringstream ss;
        // ss << "../examples/decompressed/chunk_";
        // ss << x << ".";
        // ss << z << ".dat";
        //
        // std::ofstream o(ss.str(), std::ios::binary);
        // if (!o) {
        //     throw std::ios_base::failure("Failed to open file");
        // }
        //
        // o.write(reinterpret_cast<const char*>(chunkData.data()),
        // chunkData.size()); if (!o) {
        //     throw std::ios_base::failure("Failed to write");
        // }

        this->version = chunkIO.read<uint16_t>(byteOrder);

        DebugLog("Version: " << this->version);

        switch (this->version) {
        case 12:
            this->readV12(chunkData, byteOrder);
            break;
        default:
            throw std::runtime_error("Unsupported chunk version " +
                                     std::to_string(this->version));
            break;
        }
    }

    uint16_t Chunk::getVersion() const { return this->version; }

    int32_t Chunk::getX() const { return this->x; }

    int32_t Chunk::getZ() const { return this->z; }

    uint64_t Chunk::getLastUpdate() const { return this->lastUpdate; }

    uint64_t Chunk::getInhabitedTime() const { return this->inhabitedTime; }
} // namespace lce::world
