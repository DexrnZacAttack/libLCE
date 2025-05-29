//
// Created by DexrnZacAttack on 4/3/2025.
//

#include <World/Chunk.h>

namespace lce::world {

    void Chunk::readV12(std::vector<uint8_t> data, ByteOrder endian) {
        io::BinaryIO io(data.data());

        uint16_t version = io.read<uint16_t>(endian);
        this->x = io.read<int32_t>(endian);
        this->z = io.read<int32_t>(endian);

        DebugLog("x: " << this->x << ", z: " << this->z);

        this->lastUpdate = io.read<uint64_t>(endian);
        this->inhabitedTime = io.read<uint64_t>(endian);

        uint16_t blkSize = io.read<uint16_t>(endian) * 0x100;

        std::vector<uint16_t> jumps(16);
        std::vector<uint16_t> sizes(16);

        for (uint16_t i = 0; i < 16; i++) {
            jumps[i] = io.read<uint16_t>(endian);
        }

        for (uint16_t i = 0; i < 16; i++) {
            sizes[i] = io.read<uint16_t>(endian) * 0x100;
        }

        // todo: continue later
    }

} // lce::world::chunk