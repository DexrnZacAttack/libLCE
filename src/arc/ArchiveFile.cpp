//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "../io/BinaryIO.h"
#include "ArchiveFile.h"

namespace lce::arc {
    ArchiveFile::ArchiveFile() {
    }

    ArchiveFile::~ArchiveFile() {
    }

    ArchiveFile ArchiveFile::readArchiveFile(uint8_t *data) {
        ArchiveFile af;
        io::BinaryIO io(data);

        uint16_t name_size = io.readB<uint16_t>();

        af.name = io.readUtf8(name_size);
        af.offset = io.readB<uint32_t>();
        af.size = io.readB<uint32_t>();
        return af;
    }

    ArchiveFile ArchiveFile::readArchiveFileBIO(io::BinaryIO& io) {
        ArchiveFile af;

        uint16_t name_size = io.readB<uint16_t>();

        af.name = io.readUtf8(name_size);
        af.offset = io.readB<uint32_t>();
        af.size = io.readB<uint32_t>();
        return af;
    }
}