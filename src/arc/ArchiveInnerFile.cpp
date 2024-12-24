//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "../io/BinaryIO.h"
#include "ArchiveInnerFile.h"

namespace lce::arc {
    ArchiveInnerFile::ArchiveInnerFile() {
    }

    ArchiveInnerFile::~ArchiveInnerFile() {
    }

    ArchiveInnerFile ArchiveInnerFile::readArchiveFile(uint8_t *data) {
        ArchiveInnerFile af;
        io::BinaryIO io(data);

        uint16_t name_size = io.readB<uint16_t>();
        std::cout << name_size << std::endl;

        af.name = io.readUtf8(name_size);
        af.offset = io.readB<uint32_t>();
        af.size = io.readB<uint32_t>();
        return af;
    }

    // TODO: better way to do this (maybe not have separate function to pass io reference)
    ArchiveInnerFile ArchiveInnerFile::readArchiveFileBIO(io::BinaryIO& io) {
        ArchiveInnerFile af;

        uint16_t name_size = io.readB<uint16_t>();

        af.name = io.readUtf8(name_size);
        af.offset = io.readB<uint32_t>();
        af.size = io.readB<uint32_t>();
        return af;
    }
}