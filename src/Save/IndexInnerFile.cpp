//
// Created by DexrnZacAttack on 2/3/2025.
//

#include "IndexInnerFile.h"
#include "../IO/BinaryIO.h"

namespace lce::save {
    IndexInnerFile::IndexInnerFile() = default;

    IndexInnerFile::IndexInnerFile(uint8_t* data, bool readOld, ByteOrder endian): endian(endian) {
        io::BinaryIO io(data);
        this->name = io.readWChar2Byte(64, this->endian);
        this->size = io.read<uint32_t>(this->endian);
        this->offset = io.read<uint32_t>(this->endian);
        this->timestamp = 0;

        if (!readOld)
            this->timestamp = io.read<uint64_t>(this->endian);
    }

    std::u16string IndexInnerFile::getName() const {
        return this->name;
    }

    void IndexInnerFile::setName(const std::u16string &name) {
        this->name = name;
    }

    uint32_t IndexInnerFile::getSize() const {
        return this->size;
    }

    uint32_t IndexInnerFile::getOffset() const {
        return this->offset;
    }

    void IndexInnerFile::setOffset(uint32_t offset) {
        this->offset = offset;
    }

    uint64_t IndexInnerFile::getTimestamp() const {
        return this->timestamp;
    }

    void IndexInnerFile::setTimestamp(uint64_t timestamp) {
        this->timestamp = timestamp;
    }

    uint8_t * IndexInnerFile::getData() const {
        return this->data;
    }

    std::vector<uint8_t> IndexInnerFile::getDataVec() const {
        return std::vector<uint8_t>(this->data, this->data + this->size);
    }

    void IndexInnerFile::setDataVec(std::vector<uint8_t> vector) {
        this->data = vector.data();
    }

    void IndexInnerFile::setData(uint8_t *data) {
        this->data = data;
    }

    ByteOrder IndexInnerFile::getEndian() const {
        return this->endian;
    }

    void IndexInnerFile::setEndian(ByteOrder endian) {
        this->endian = endian;
    }
}
