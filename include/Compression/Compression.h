//
// Created by DexrnZacAttack on 12/28/2024.
//

#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <vector>

#include <libLCE.h>
#include <IO/ByteEnums.h>

namespace lce::compression {

enum LIBLCE_API CompressionType {
    ZLIB,
    LZX,
    SPLIT_SAVE,
    CHUNK,
    VITA,
    DEFLATE // different from zlib
};

class LIBLCE_API Compression {
public:
    static bool decompressZlib(std::vector<uint8_t> &in, std::vector<uint8_t> &out);

    static bool decompressZlibWithLength(std::vector<uint8_t> &in, std::vector<uint8_t> &out, uint32_t bufSize, uint32_t offset);

    static bool decompressChunk(std::vector<uint8_t> &in, std::vector<uint8_t> &out);

    static bool decompressVita(std::vector<uint8_t> &in, std::vector<uint8_t> &out, uint32_t outBuf, uint32_t offset = 0);

    static bool decompress(std::vector<uint8_t> &in, std::vector<uint8_t> &out, compression::CompressionType type);

    static uint32_t getSizeFromSave(std::vector<uint8_t> &in, ByteOrder endian);
};

} // lce::compression

#endif //COMPRESSION_H
