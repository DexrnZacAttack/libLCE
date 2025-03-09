//
// Created by DexrnZacAttack on 12/28/2024.
//

#ifndef COMPRESSION_H
#define COMPRESSION_H

namespace lce::compression {

enum CompressionType {
    ZLIB,
    LZX,
    SPLIT_SAVE,
    CHUNK,
    VITA,
    DEFLATE // different from zlib
};

class Compression {

};

} // lce::compression

#endif //COMPRESSION_H
