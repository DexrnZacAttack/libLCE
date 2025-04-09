//
// Created by DexrnZacAttack on 12/28/2024.
//

#ifndef CHUNK_H
#define CHUNK_H
#include <cstdint>
#include <vector>

#include "Region.h"
#include "../IO/BinaryIO.h"
#include "../lce.h"

#include "../Compression/Compression.h"

namespace lce::world {
    // TODO: chunk as base with subclasses for each version
    class LIBLCE_API Chunk {
    protected:
        uint16_t version;
        int32_t x;
        int32_t z;
        uint64_t lastUpdate;
        uint64_t inhabitedTime;
    public:
        class LIBLCE_API Section {
        public:
            std::vector<uint16_t> blocks; // blocks

            Section();
        };

        Chunk();

        Chunk(std::vector<uint8_t> data, compression::CompressionType outerCompression, ByteOrder endian = ByteOrder::LITTLE);

        Chunk(uint8_t *data, compression::CompressionType outerCompression, ByteOrder endian);

        // region Readers

        void readV12(std::vector<uint8_t> data, ByteOrder endian);

        // endregion

        // region Writers

        // endregion

        // region Getters

        [[nodiscard]] uint16_t getVersion() const;
        [[nodiscard]] int32_t getX() const;
        [[nodiscard]] int32_t getZ() const;
        [[nodiscard]] uint64_t getLastUpdate() const;
        [[nodiscard]] uint64_t getInhabitedTime() const;

        // endregion
    };
} // lce::world

#endif //CHUNK_H
