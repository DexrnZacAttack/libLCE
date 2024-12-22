//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "IndexInnerFile.h"
#include "../io/BinaryIO.h"

namespace lce::save {
    IndexInnerFile::IndexInnerFile() {
    }

    IndexInnerFile::~IndexInnerFile() {
    }

    IndexInnerFile IndexInnerFile::readInnerFile(uint8_t *data) {
        IndexInnerFile inf;
        io::BinaryIO io(data);
        inf.name = io.readWChar2ByteB(64);
        inf.size = io.readB<uint32_t>();
        inf.offset = io.readB<uint32_t>();
        inf.timestamp = io.readB<uint64_t>();
        return inf;
    }

    IndexInnerFile IndexInnerFile::readOldInnerFile(uint8_t *data) {
        IndexInnerFile inf;
        io::BinaryIO io(data);
        inf.name = io.readWChar2ByteB(64);
        inf.size = io.readB<uint32_t>();
        inf.offset = io.readB<uint32_t>();
        return inf;
    }
}