//
// Created by DexrnZacAttack on 12/28/2024.
//

#include "BinaryIO.h"
#include <codecvt>
#include <locale>
#include <string>

namespace lce::io {
    std::string BinaryIO::readUtf8(size_t size) {
        uint8_t* buf = readOfSize(size);
        std::string result(reinterpret_cast<char*>(buf), size);
        delete[] buf;
        return result;
    }
    
    std::string BinaryIO::readUtf8NullTerminated() {
        const char* start = reinterpret_cast<const char*>(this->data);

        size_t len = 0;
        while (this->data[len] != 0) ++len;

        std::string result(start, len);

        this->data += len + 1;
        return result;
    }

    void BinaryIO::writeUtf8(const std::string& input) {
        writeBytes(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    }

    void BinaryIO::trimWString(std::wstring& input) {
        size_t len = input.size();
        while (len > 0 && input[len - 1] == L'\0') {
            --len;
        }
        input.resize(len);
    }

    std::u16string BinaryIO::readWChar2Byte(const size_t size, ByteOrder endian) {
        std::u16string result(size, u'\0');
        std::memcpy(&result[0], this->data, size * sizeof(char16_t));
        this->data += size * sizeof(char16_t);

#if defined(BR_BIG_ENDIAN)
        if (endian == ByteOrder::LITTLE) {
#else
        if (endian == ByteOrder::BIG) {
#endif
            for (size_t i = 0; i < size; ++i) {
                result[i] = swapOrder(result[i]);
            }
        }

        return result;

    }

    std::u16string BinaryIO::readWChar2ByteNT(ByteOrder endian) {
        const char16_t* ptr = reinterpret_cast<const char16_t*>(this->data);
        size_t len = 0;
        while (ptr[len] != 0) ++len;

        auto result = readWChar2Byte(len, endian);
        this->data += (len + 1) * sizeof(char16_t);
        return result;
    }

    void BinaryIO::writeWChar2Byte(const std::u16string& input, ByteOrder endian, bool nullTerminate = false) {
        for (const char16_t& ch : input) {
            this->write<char16_t>(ch, endian);
        }

        if (nullTerminate)
            this->write<char16_t>(0, endian);
    }

    std::u32string BinaryIO::readWChar4Byte(const size_t size,ByteOrder endian) {
        std::u32string result(size, u'\0');
        std::memcpy(&result[0], this->data, size * sizeof(char32_t));
        this->data += size * sizeof(char32_t);

#if defined(BR_BIG_ENDIAN)
        if (endian == ByteOrder::LITTLE) {
#else
        if (endian == ByteOrder::BIG) {
#endif
            for (size_t i = 0; i < size; ++i) {
                result[i] = swapOrder(result[i]);
            }
        }

        return result;
    }

    std::u32string BinaryIO::readWChar4ByteNT(ByteOrder endian) {
        const char32_t* ptr = reinterpret_cast<const char32_t*>(this->data);
        size_t len = 0;
        while (ptr[len] != 0) ++len;

        auto result = readWChar4Byte(len, endian);
        this->data += (len + 1) * sizeof(char32_t);
        return result;
    }

    void BinaryIO::writeWChar4Byte(const std::u32string& input, ByteOrder endian, bool nullTerminate = false) {
        for (const char32_t& ch : input) {
            this->write<char32_t>(ch, endian);
        }

        if (nullTerminate)
            this->write<char32_t>(0, endian);
    }

    // region Converters

    std::wstring BinaryIO::u16stringToWstring(const std::u16string& str) {
#if WCHAR_MAX == 0xffff
        return std::wstring(str.begin(), str.end());
#else
        std::wstring result;
        result.reserve(str.size());

        for (char16_t c : str) {
            result.push_back(static_cast<wchar_t>(c));
        }
        return result;
#endif

    }


    std::u16string BinaryIO::wstringToU16string(const std::wstring& str) {
#if WCHAR_MAX == 0xffff
        return std::u16string(str.begin(), str.end());
#else
        std::u16string result;
        result.reserve(str.size());

        for (wchar_t c : str) {
            result.push_back(static_cast<char16_t>(c));
        }
        return result;
#endif
    }

    std::wstring BinaryIO::u32stringToWstring(const std::u32string& str) {
#if WCHAR_MAX == 0x7fffffff
            return std::wstring(str.begin(), str.end());
#else
        // LOSSY LOSSY LOSSY LOSSY LOSSY AAAAAAAAAAAAA
        std::wstring result;
        result.reserve(str.size());

        for (char32_t c : str) {
            result.push_back(static_cast<wchar_t>(c));
        }
        return result;
#endif
    }

    std::string BinaryIO::u16stringToString(const std::u16string &str) {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        return convert.to_bytes(str);
    }
    
    std::u16string BinaryIO::stringToU16String(const std::string &str) {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        return convert.from_bytes(str);
    }


    std::string BinaryIO::wstringToString(const std::wstring& str) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
        return convert.to_bytes(str);
    }


    std::wstring BinaryIO::stringToWString(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
        return convert.from_bytes(str);
    }

    // endregion

} // lce
