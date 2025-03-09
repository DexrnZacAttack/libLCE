//
// Created by DexrnZacAttack on 12/28/2024.
//

#include "BinaryIO.h"

namespace lce::io {
    std::string BinaryIO::readUtf8(size_t size) {
        uint8_t* buf = readOfSize(size);
        return {reinterpret_cast<char*>(buf), size};
    }

    void BinaryIO::writeUtf8(const std::string& input) {
        for (const char& ch : input) {
            this->writeBE<uint8_t>(static_cast<uint8_t>(ch));
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

    std::wstring BinaryIO::readWChar2Byte(const size_t size, ByteOrder endian) {
        std::wstring result;
        for (size_t i = 0; i < size; i++) {
            result += static_cast<wchar_t>(this->read<uint16_t>(endian));
        }
        return result;
    }

    void BinaryIO::writeWChar2Byte(const std::wstring& input, ByteOrder endian) {
        for (const wchar_t& ch : input) {
            this->write<uint16_t>(static_cast<uint16_t>(ch), endian);
        }
    }

    std::wstring BinaryIO::readWChar4ByteB(const size_t size) {
        std::wstring result;
        for (size_t i = 0; i < size; i++) {
            result += static_cast<wchar_t>(this->readBE<uint32_t>());
        }
        return result;
    }

    std::wstring BinaryIO::readWChar4ByteL(const size_t size) {
        std::wstring result;
        for (size_t i = 0; i < size; i++) {
            result += static_cast<wchar_t>(this->readLE<uint32_t>());
        }
        return result;
    }

    void BinaryIO::writeWChar4ByteB(const std::wstring& input) {
        for (const wchar_t& ch : input) {
            this->writeBE<uint32_t>(static_cast<uint32_t>(ch));
        }
    }

    void BinaryIO::writeWChar4ByteL(const std::wstring& input) {
        for (const wchar_t& ch : input) {
            this->writeLE<uint32_t>(static_cast<uint32_t>(ch));
        }
    }
} // lce