//
// Created by DexrnZacAttack on 4/3/2025.
//

#include <World/Chunk.h>

#include "World/Block.h"

namespace lce::world {

    void Chunk::readV12(std::vector<uint8_t> data, io::ByteOrder endian) {
        io::BinaryIO io(data.data());

        this->version = io.read<uint16_t>(endian);
        this->x = io.read<int32_t>(endian);
        this->z = io.read<int32_t>(endian);

        DebugLog("x: " << this->x << ", z: " << this->z);

        this->lastUpdate = io.read<uint64_t>(endian);
        this->inhabitedTime = io.read<uint64_t>(endian);

        uint16_t sectionsSize = io.read<uint16_t>(endian);
        uint16_t sectionsSizeInBytes = sectionsSize * 0x100;

        std::vector<int16_t> jumps(16);
        std::vector<int8_t> sizes(16);

        for (int16_t i = 0; i < 16; i++) {
            jumps[i] = io.read<int16_t>(endian);
        }

        for (int8_t i = 0; i < 16; i++) {
            sizes[i] = io.read<int8_t>(endian);
        }

        // literally we have been working on this shit for hours and can't figure it out... Below is what was kept of
        // PhoenixVX's work (as we should *probably* start fresh tomorrow).
        // Read each section
        for (int i = 0; i < 16; i++) {
            int jump = jumps[i];
            int size = sizes[i] * 0x100;

            if (size == 0)
                continue;

            // Read all data from chunk section
            io.seek(0x4C + jump);
        }
    }

} // namespace lce::world
