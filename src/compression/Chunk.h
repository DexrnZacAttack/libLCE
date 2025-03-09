//
// Created by DexrnZacAttack on 12/28/2024.
//

#ifndef COMPRESSION_CHUNK_H
#define COMPRESSION_CHUNK_H

#include <utility>
#include <vector>

namespace lce::compression {

class Chunk {
    public:
    static std::vector<uint8_t> compress(std::vector<uint8_t> input);
    static uint8_t* compress(uint8_t *data, size_t decompressedSize);
    static std::vector<uint8_t> decompress(std::vector<uint8_t> input);
    static uint8_t* decompress(uint8_t *data, size_t compressedSize);
};

} // lce::compression

#endif //COMPRESSION_CHUNK_H
