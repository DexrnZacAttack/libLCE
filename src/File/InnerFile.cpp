//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "../IO/BinaryIO.h"
#include "InnerFile.h"

namespace lce::file {
    InnerFile::InnerFile() {
    }

    InnerFile::~InnerFile() {
    }

    InnerFile::InnerFile(uint8_t *data) {
        io::BinaryIO io(data);

        uint16_t name_size = io.readBE<uint16_t>();
        std::cout << name_size << std::endl;

        this->name = io.readUtf8(name_size);
        this->offset = io.readBE<uint32_t>();
        this->size = io.readBE<uint32_t>();
    }

    // TODO: better way to do this (maybe not have separate function to pass io reference)
	InnerFile::InnerFile (io::BinaryIO& io) {
        uint16_t name_size = io.readBE<uint16_t>();

        this->name = io.readUtf8(name_size);
        this->offset = io.readBE<uint32_t>();
        this->size = io.readBE<uint32_t>();
    }
    
    uint8_t* InnerFile::create() const {
		return data;
	}
}
