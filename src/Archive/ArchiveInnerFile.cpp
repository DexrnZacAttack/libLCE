//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "../IO/BinaryIO.h"
#include "ArchiveInnerFile.h"

namespace lce::arc {
    ArchiveInnerFile::ArchiveInnerFile() {
    }

    ArchiveInnerFile::~ArchiveInnerFile() {
    }

    ArchiveInnerFile::ArchiveInnerFile(uint8_t *data) {
        io::BinaryIO io(data);

        uint16_t name_size = io.readBE<uint16_t>();
        std::cout << name_size << std::endl;

        this->name = io.readUtf8(name_size);
        this->offset = io.readBE<uint32_t>();
        this->size = io.readBE<uint32_t>();
    }

    // TODO: better way to do this (maybe not have separate function to pass io reference)
    ArchiveInnerFile::ArchiveInnerFile (io::BinaryIO& io) {
        uint16_t name_size = io.readBE<uint16_t>();

        this->name = io.readUtf8(name_size);
        this->offset = io.readBE<uint32_t>();
        this->size = io.readBE<uint32_t>();
    }
}