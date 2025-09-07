//
// Created by (Person who prefers not to be named) and DexrnZacAttack on
// 20/12/2024.
//

#include <IO/BinaryIO.h>

#include <cstring>
#include <vector>

namespace lce::io {
    BinaryIO::BinaryIO(uint8_t *input, const size_t size)
        : mOrigin(input), mData(input), mSize(size) {}

    BinaryIO::BinaryIO(const size_t size) {
        uint8_t *data = new uint8_t[size]{};
        this->mOrigin = data;
        this->mData = data;
        this->mSize = size;
    }

    void BinaryIO::seek(const size_t offset) {
        this->mData = this->mOrigin + offset;
    }

    void BinaryIO::seekRelative(const size_t offset) { this->mData += offset; }

    size_t BinaryIO::getPosition() const { return this->mData - this->mOrigin; }

    uint8_t BinaryIO::readByte() { return *this->mData++; }

    int8_t BinaryIO::readSignedByte() {
        return static_cast<int8_t>(*this->mData++);
    }

    uint32_t BinaryIO::readUint24(const io::ByteOrder endian) {
        const uint8_t b0 = readByte();
        const uint8_t b1 = readByte();
        const uint8_t b2 = readByte();

        if (endian == ByteOrder::LITTLE)
            return static_cast<uint32_t>(b0) |
                   (static_cast<uint32_t>(b1) << 8) |
                   (static_cast<uint32_t>(b2) << 16);

        return (static_cast<uint32_t>(b0) << 16) |
               (static_cast<uint32_t>(b1) << 8) | static_cast<uint32_t>(b2);
    }

    int32_t BinaryIO::readInt24(const ByteOrder endian) {
        uint32_t res = readUint24(endian);

        if (res & (1 << 23)) {
            res |= 0xFF000000;
        }

        return static_cast<int32_t>(res);
    }

    void BinaryIO::writeByte(const uint8_t v) { *this->mData++ = v; }

    void BinaryIO::writeBytes(const uint8_t *v, const size_t size) {
        std::memcpy(this->mData, v, size);
        this->mData += size;
    }

    void BinaryIO::writeSignedByte(const int8_t v) {
        *this->mData++ = static_cast<uint8_t>(v);
    }

    uint8_t *BinaryIO::getData() const { return this->mOrigin; }

    uint8_t *BinaryIO::getDataRelative() const { return this->mData; }

    uint8_t *&BinaryIO::getDataRef() { return this->mOrigin; }

    uint8_t *&BinaryIO::getDataRefRelative() { return this->mData; }

    uint8_t *BinaryIO::readOfSize(const size_t sz) {
        uint8_t *result = new uint8_t[sz];
        std::memcpy(result, this->mData, sz);
        this->mData += sz;
        return result;
    }

    std::vector<uint8_t> BinaryIO::readOfSizeVec(const size_t sz) {
        std::vector result(this->mData, this->mData + sz);
        this->mData += sz;
        return result;
    }

    void BinaryIO::readInto(uint8_t *into, const size_t sz) {
        std::memcpy(into, this->mData, sz);
        this->mData += sz;
    }

} // namespace lce::io
