//
// Created by DexrnZacAttack on 2/3/2025.
//

#include "IndexInnerFile.h"
#include "../IO/BinaryIO.h"

namespace lce::save {
    IndexInnerFile::IndexInnerFile() = default;

    IndexInnerFile::IndexInnerFile(uint8_t* data, bool readOld, ByteOrder endian): endian(endian) {
        io::BinaryIO io(data);
        setNameU16(io.readWChar2Byte(64, this->endian));
        setSize(io.read<uint32_t>(this->endian));
        setOffset(io.read<uint32_t>(this->endian));
        this->timestamp = 0;
        
        setData(data);
    }

    void IndexInnerFile::setTimestamp(uint64_t timestamp) {
        this->timestamp = timestamp;
    }
    
    uint64_t IndexInnerFile::getTimestamp() const {
        return this->timestamp;
    }


    void IndexInnerFile::setEndian(ByteOrder endian) {
        this->endian = endian;
    }
    
    ByteOrder IndexInnerFile::getEndian() const {
        return this->endian;
    }
}
