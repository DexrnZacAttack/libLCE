//
// Created by DexrnZacAttack on 2/3/2025.
//

#include "IndexInnerFile.h"
#include "../io/BinaryIO.h"

namespace lce::save {
    IndexInnerFile::IndexInnerFile() = default;

    IndexInnerFile::IndexInnerFile(uint8_t *data, bool readOld, ByteOrder endian): endian(endian) {
        io::BinaryIO io(data);
        this->name = io.readWChar2Byte(64, this->endian);
        this->size = io.read<uint32_t>(this->endian);
        this->offset = io.read<uint32_t>(this->endian);
        if (readOld)
            this->timestamp = io.read<uint64_t>(this->endian);
    }

}