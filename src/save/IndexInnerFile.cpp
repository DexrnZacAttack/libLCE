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

    IndexInnerFile IndexInnerFile::read(uint8_t *data) {
        IndexInnerFile inf;
        io::BinaryIO io(data);
        inf.name = io.readWChar2ByteB(64);
        io::BinaryIO::trimWString(inf.name);
        inf.size = io.readB<uint32_t>();
        inf.offset = io.readB<uint32_t>();
        inf.timestamp = io.readB<uint64_t>();
        return inf;
    }

    IndexInnerFile IndexInnerFile::readOld(uint8_t *data) {
        IndexInnerFile inf;
        io::BinaryIO io(data);
        inf.name = io.readWChar2ByteB(64);
        io::BinaryIO::trimWString(inf.name);
        inf.size = io.readB<uint32_t>();
        inf.offset = io.readB<uint32_t>();
        return inf;
    }
}