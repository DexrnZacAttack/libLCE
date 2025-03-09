//
// Created by (Person who prefers not to be named) and DexrnZacAttack on 20/12/2024.
//

#include "BinaryIO.h"

#include <vector>
#include <cstring>
#include <iostream>

namespace lce::io {
    BinaryIO::BinaryIO(uint8_t *input, size_t size) : dataOrigin(input), data(input), size(size) {
    }

    BinaryIO::BinaryIO(size_t size) {
        auto *data = new uint8_t[size]{};
        this->dataOrigin = data;
        this->data = data;
        this->size = size;
    }

    // BinaryIO::~BinaryIO() {
    //     delete[] data;
    //     delete[] dataOrigin;
    // }

    void BinaryIO::seek(const size_t offset) {
        this->data = this->dataOrigin+offset;
    }

    void BinaryIO::seekRelative(const size_t offset) {
        this->data += offset;
    }

    size_t BinaryIO::getPosition() const {
        return this->data - this->dataOrigin;
    }

    uint8_t BinaryIO::readByte() {
        return *this->data++;
    }

    int8_t BinaryIO::readSignedByte() {
        return static_cast<int8_t>(*this->data++);
    }

    void BinaryIO::writeByte(const uint8_t v) {
        *this->data++ = v;
    }

    void BinaryIO::writeBytes(const uint8_t* v, const size_t size) {
        std::memcpy(this->data, v, size);
        this->data += size;
    }

    void BinaryIO::writeSignedByte(const int8_t v) {
        *this->data++ = static_cast<uint8_t>(v);
    }

    uint8_t* BinaryIO::getData() {
        return this->dataOrigin;
    }

    uint8_t *BinaryIO::getDataRelative() const {
        return this->data;
    }

    uint8_t*& BinaryIO::getDataRef() {
        return this->dataOrigin;
    }

    uint8_t*& BinaryIO::getDataRefRelative() {
        return this->data;
    }

    uint8_t* BinaryIO::readOfSize(size_t size) {
        uint8_t* result = new uint8_t[size];

        for (size_t i = 0; i < size; i++) {
            result[i] = *this->data++;
        }

        return result;
    }

    std::vector<uint8_t> BinaryIO::readOfSizeVec(size_t size) {
        std::vector<uint8_t> result = std::vector<uint8_t>(size);

        for (size_t i = 0; i < size; i++) {
            result.push_back(*this->data++);
        }

        return result;
    }

    void BinaryIO::readInto(uint8_t* into, size_t size) {
        for (size_t i = 0; i < size; i++) {
            into[i] = *this->data++;
        }
    }

}
