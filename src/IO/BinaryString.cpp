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
        return {reinterpret_cast<char*>(buf), size};
    }
    
    std::string BinaryIO::readUtf8NullTerminated() {
		std::string result;
		
		while (true) {
			char ch = *this->data++;
			if(ch == '\0') break;
			result += ch;
		}
		
		return result;
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

    std::u16string BinaryIO::readWChar2Byte(const size_t size, ByteOrder endian) {
        std::u16string result;
        for (size_t i = 0; i < size; i++) {
            result += this->read<char16_t>(endian);
        }
        return result;
    }

    std::u16string BinaryIO::readWChar2ByteNT(ByteOrder endian) {
        std::u16string result;
        char16_t read;

        while ((read = this->read<char16_t>(endian)) != u'\0') {
            result += read;
        }

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
        std::u32string result;
        for (size_t i = 0; i < size; i++) {
            result += this->read<char32_t>(endian);
        }
        return result;
    }

    std::u32string BinaryIO::readWChar4ByteNT(ByteOrder endian) {
        std::u32string result;
        char32_t read;

        while ((read = this->read<char32_t>(endian)) != U'\0') {
            result += read;
        }

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

    // endregion

} // lce
