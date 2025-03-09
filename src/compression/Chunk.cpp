//
// Created by DexrnZacAttack on 12/28/2024.
//

#include "Chunk.h"
#include "../io/BinaryIO.h"

namespace lce::compression {
    std::vector<uint8_t> Chunk::compress(std::vector<uint8_t> data) {
        std::vector<uint8_t> result;
        return result;
    }

    uint8_t* Chunk::compress(uint8_t *data, size_t decompressedSize) {
        return nullptr;
    }

    std::vector<uint8_t> Chunk::decompress(std::vector<uint8_t> data) {
        io::BinaryIO io(data.data());
        std::vector<uint8_t> result;

        while (io.getPosition() < data.size()) {
            uint8_t byte = io.readByte();

            if (byte != 0xFF) {
                result.push_back(byte);
            } else {
                uint8_t val = 255;
                uint8_t count = io.readByte();

                if (count >= 3) {
                    val = io.readByte();
                }

                result.insert(result.end(), count + 1, val);
            }
        }

        return result;
    }

    uint8_t* Chunk::decompress(uint8_t *data, size_t compressedSize) {
        return nullptr;
    }
} // lce::compression