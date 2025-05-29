//
// Created by (Person who prefers not to be named) and DexrnZacAttack on 20/12/2024.
//

#include <IO/BinaryIO.h>

#include <cstring>
#include <vector>

namespace lce::io {
    BinaryIO::BinaryIO(uint8_t* input, size_t size) : dataOrigin(input), data(input), size(size) {}

    BinaryIO::BinaryIO(size_t size) {
        auto* data = new uint8_t[size]{};
        this->dataOrigin = data;
        this->data = data;
        this->size = size;
    }

    void BinaryIO::seek(const size_t offset) { this->data = this->dataOrigin + offset; }

    void BinaryIO::seekRelative(const size_t offset) { this->data += offset; }

    size_t BinaryIO::getPosition() const { return this->data - this->dataOrigin; }

    uint8_t BinaryIO::readByte() { return *this->data++; }

    int8_t BinaryIO::readSignedByte() { return static_cast<int8_t>(*this->data++); }

    uint32_t BinaryIO::readUint24(io::ByteOrder endian) {
        const uint8_t b0 = readByte();
        const uint8_t b1 = readByte();
        const uint8_t b2 = readByte();

        if (endian == io::ByteOrder::LITTLE)
            return static_cast<uint32_t>(b0) | (static_cast<uint32_t>(b1) << 8) | (static_cast<uint32_t>(b2) << 16);

        return (static_cast<uint32_t>(b0) << 16) | (static_cast<uint32_t>(b1) << 8) | static_cast<uint32_t>(b2);
    }


    int32_t BinaryIO::readInt24(io::ByteOrder endian) {
        uint32_t res = readUint24(endian);

        if (res & (1 << 23)) {
            res |= 0xFF000000;
        }

        return static_cast<int32_t>(res);
    }

    void BinaryIO::writeByte(const uint8_t v) { *this->data++ = v; }

    void BinaryIO::writeBytes(const uint8_t* v, const size_t size) {
        std::memcpy(this->data, v, size);
        this->data += size;
    }

    void BinaryIO::writeSignedByte(const int8_t v) { *this->data++ = static_cast<uint8_t>(v); }

    uint8_t* BinaryIO::getData() { return this->dataOrigin; }

    uint8_t* BinaryIO::getDataRelative() const { return this->data; }

    uint8_t*& BinaryIO::getDataRef() { return this->dataOrigin; }

    uint8_t*& BinaryIO::getDataRefRelative() { return this->data; }

    uint8_t* BinaryIO::readOfSize(size_t size) {
        uint8_t* result = new uint8_t[size];
        std::memcpy(result, this->data, size);
        this->data += size;
        return result;
    }


    std::vector<uint8_t> BinaryIO::readOfSizeVec(size_t size) {
        std::vector<uint8_t> result(size);
        std::memcpy(result.data(), this->data, size);
        this->data += size;
        return result;
    }


    void BinaryIO::readInto(uint8_t* into, size_t size) {
        std::memcpy(into, this->data, size);
        this->data += size;
    }


} // namespace lce::io
