//
// Created by (Person who prefers not to be named) and DexrnZacAttack on 20/12/2024.
//

#include "BinaryIO.h"

#include <vector>

namespace lce::io {
    BinaryIO::BinaryIO(uint8_t *input) : dataOrigin(input), data(input) {
    }

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

    void BinaryIO::readInto(uint8_t* into, size_t size) {
        for (size_t i = 0; i < size; i++) {
            into[i] = *this->data++;
        }
    }

    std::string BinaryIO::readUtf8(size_t size) {
        uint8_t* buf = readOfSize(size);
        return {reinterpret_cast<char*>(buf), size};
    }

    void BinaryIO::writeUtf8(const std::string& input) {
        for (const char& ch : input) {
            this->writeB<uint8_t>(static_cast<uint8_t>(ch));
        }
    }


    void BinaryIO::trimWString(std::wstring& inp) {
        auto pos = inp.find_last_not_of(L'\0');
        if (pos != std::wstring::npos) {
            inp.resize(pos + 1);
        } else {
            inp.clear();
        }
    }

    std::wstring BinaryIO::readWChar2ByteB(const size_t size) {
        std::wstring result;
        for (size_t i = 0; i < size; i++) {
            result += static_cast<wchar_t>(this->readB<uint16_t>());
        }
        return result;
    }

    std::wstring BinaryIO::readWChar2ByteL(const size_t size) {
        std::wstring result;
        for (size_t i = 0; i < size; i++) {
            result += static_cast<wchar_t>(this->readL<uint16_t>());
        }
        return result;
    }

    void BinaryIO::writeWChar2ByteB(const std::wstring& input) {
        for (const wchar_t& ch : input) {
            this->writeB<uint16_t>(static_cast<uint16_t>(ch));
        }
    }

    void BinaryIO::writeWChar2ByteL(const std::wstring& input) {
        for (const wchar_t& ch : input) {
            this->writeL<uint16_t>(static_cast<uint16_t>(ch));
        }
    }

    std::wstring BinaryIO::readWChar4ByteB(const size_t size) {
        std::wstring result;
        for (size_t i = 0; i < size; i++) {
            result += static_cast<wchar_t>(this->readB<uint32_t>());
        }
        return result;
    }

    std::wstring BinaryIO::readWChar4ByteL(const size_t size) {
        std::wstring result;
        for (size_t i = 0; i < size; i++) {
            result += static_cast<wchar_t>(this->readL<uint32_t>());
        }
        return result;
    }

    void BinaryIO::writeWChar4ByteB(const std::wstring& input) {
        for (const wchar_t& ch : input) {
            this->writeB<uint32_t>(static_cast<uint32_t>(ch));
        }
    }

    void BinaryIO::writeWChar4ByteL(const std::wstring& input) {
        for (const wchar_t& ch : input) {
            this->writeL<uint32_t>(static_cast<uint32_t>(ch));
        }
    }
}
