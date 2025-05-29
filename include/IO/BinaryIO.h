//
// Created by (Person who prefers not to be named) and DexrnZacAttack on 20/12/2024.
//

#ifndef BINARYIO_H
#define BINARYIO_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <IO/ByteEnums.h>
#include <libLCE.h>


namespace lce::io {
    template <typename T>
    constexpr T swapOrder_f(const T input) {
        uint8_t resultBytes[sizeof(T)];
        const uint8_t* ib = reinterpret_cast<const uint8_t*>(&input);
        uint8_t* r = resultBytes + (sizeof(T) - 1);

        for (size_t i = 0; i < sizeof(T); ++i) {
            *r-- = *ib++;
        }

        T result;
        memcpy(&result, resultBytes, sizeof(T));
        return result;
    }

    // this is a mess but theoretically will actually use the byteswap instruction
    template <typename T>
constexpr T swapOrder(const T input) {
        if constexpr (std::is_integral_v<T>) {
            if constexpr (sizeof(T) == 2) {
#if defined(__clang__) || defined(__GNUC__)
                return static_cast<T>(__builtin_bswap16(static_cast<uint16_t>(input)));
#elif defined(_MSC_VER)
                return static_cast<T>(_byteswap_ushort(static_cast<unsigned short>(input)));
#else
                return swapOrder_f(input);
#endif
            } else if constexpr (sizeof(T) == 4) {
#if defined(__clang__) || defined(__GNUC__)
                return static_cast<T>(__builtin_bswap32(static_cast<uint32_t>(input)));
#elif defined(_MSC_VER)
                return static_cast<T>(_byteswap_ulong(static_cast<unsigned long>(input)));
#else
                return swapOrder_f(input);
#endif
            } else if constexpr (sizeof(T) == 8) {
#if defined(__clang__) || defined(__GNUC__)
                return static_cast<T>(__builtin_bswap64(static_cast<uint64_t>(input)));
#elif defined(_MSC_VER)
                return static_cast<T>(_byteswap_uint64(static_cast<unsigned __int64>(input)));
#else
                return swapOrder_f(input);
#endif
            } else {
                return swapOrder_f(input);
            }
        } else {
            return swapOrder_f(input);
        }
    }


    template <typename T>
    constexpr T big2sys(const T a) {
#ifdef BR_BIG_ENDIAN
        return a;
#else
        return swapOrder(a);
#endif
    }

    template <typename T>
    constexpr T little2sys(const T a) { //also is working as sys2little
#ifdef BR_BIG_ENDIAN
        return swapOrder(a);
#else
        return a;
#endif
    }

    // todo: implement size shits
class BinaryIO {
    private:
        uint8_t* dataOrigin;
        uint8_t* data;
        size_t size;
    public:
        explicit BinaryIO(uint8_t* input, size_t size = 0);
        explicit BinaryIO(size_t size);

        // ~BinaryIO();

        void seek(size_t offset);
        void seekRelative(size_t offset);

        //reading
        uint8_t readByte();
        int8_t readSignedByte();

        uint32_t readUint24(ByteOrder endian);

        int32_t readInt24(ByteOrder endian);

        template <typename T>
        T readLE() {
            const T v = little2sys(*reinterpret_cast<const T*>(this->data));
            this->data+=sizeof(T);
            return v;
        }

        template <typename T>
        T readBE() {
            const T v = big2sys(*reinterpret_cast<const T*>(this->data));
            this->data+=sizeof(T);
            return v;
        }

        template <typename T>
        T read(const ByteOrder endian) {
            T v;
            if (endian == LITTLE)
                v = little2sys(*reinterpret_cast<const T*>(this->data));
            else
                v = big2sys(*reinterpret_cast<const T*>(this->data));
            this->data+=sizeof(T);
            return v;
        }

        //writing
        void writeByte(uint8_t v);

        void writeBytes(const uint8_t *v, size_t size);

        void writeSignedByte(int8_t v);
        template <typename T>
        void write(const T v, const ByteOrder endian) {
            if (endian == LITTLE)
                *reinterpret_cast<T*>(this->data) = little2sys(v);
            else
                *reinterpret_cast<T*>(this->data) = big2sys(v);
            this->data+=sizeof(T);
        }

        template <typename T>
        void writeLE(const T v) {
            *reinterpret_cast<T*>(this->data) = little2sys(v);
            this->data+=sizeof(T);
        }

        template <typename T>
        void writeBE(const T v) {
            *reinterpret_cast<T*>(this->data) = big2sys(v);
            this->data+=sizeof(T);
        }

        uint8_t* getData();

        uint8_t *getDataRelative() const;

        uint8_t *&getDataRef();

        uint8_t *&getDataRefRelative();

        uint8_t *readOfSize(size_t size);

        std::vector<uint8_t> readOfSizeVec(size_t size);

        void readInto(uint8_t *into, size_t size);

        [[nodiscard]] size_t getPosition() const;

        std::string readUtf8(size_t size);
        
        std::string readUtf8NullTerminated();

        void writeUtf8(const std::string &input);

        static void trimWString(std::wstring &input);

        std::u16string readWChar2Byte(size_t size, ByteOrder endian);

        std::u16string readWChar2ByteNT(ByteOrder endian);

        void writeWChar2Byte(const std::u16string &input, ByteOrder endian, bool nullTerminate);

        std::u32string readWChar4Byte(size_t size, ByteOrder endian);

        std::u32string readWChar4ByteNT(ByteOrder endian);

        void writeWChar4Byte(const std::u32string &input, ByteOrder endian, bool nullTerminate);

        static LIBLCE_API std::wstring u16stringToWstring(const std::u16string &str);

        static LIBLCE_API std::u16string wstringToU16string(const std::wstring &str);

        static LIBLCE_API std::wstring u32stringToWstring(const std::u32string &str);
    
        static LIBLCE_API std::string u16stringToString(const std::u16string &str);
    
        static LIBLCE_API std::u16string stringToU16String(const std::string &str);
        static LIBLCE_API std::string wstringToString(const std::wstring &str);
        static LIBLCE_API std::wstring stringToWString(const std::string &str);
};
} // lce::io

#endif //BINARYIO_H
