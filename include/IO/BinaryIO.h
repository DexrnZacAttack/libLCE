//
// Created by (Person who prefers not to be named) and DexrnZacAttack on
// 20/12/2024.
//

#ifndef BINARYIO_H
#define BINARYIO_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <libLCE.h>

namespace lce::io {
    /** Byte Order/Endian dictates which way bytes are written */
    enum class ByteOrder {
        BIG,   /**< Big Endian | Writes 0xA1B2C3D4 */
        LITTLE /**< Little Endian | Writes 0xD4C3B2A1 */
    };

    /** Unsized reader/writer for a byte array */
    class BinaryIO {
      public:
        explicit BinaryIO(uint8_t *input);
        explicit BinaryIO(size_t size);

        // ~BinaryIO();

        void seek(size_t offset);
        void seekRelative(size_t offset);

        // reading
        /** Reads an unsigned byte (uint8_t)
         *
         * @returns The byte
         *
         * @see readSignedByte() for reading a signed byte
         */
        uint8_t readByte();
        /** Reads a signed byte (int8_t)
         *
         * @returns The signed byte
         *
         * @see readByte() for reading an unsigned byte
         */
        int8_t readSignedByte();

        /** Reads an unsigned 24-bit integer
         *
         * @returns The unsigned 24-bit integer
         *
         * @see readInt24() for reading a signed 24-bit integer
         */
        uint32_t readUint24(io::ByteOrder endian);

        /** Reads a signed 24-bit integer
         *
         * @returns The signed 24-bit integer
         *
         * @see readInt24() for reading an unsigned 24-bit integer
         */
        int32_t readInt24(ByteOrder endian);

        /** Reads a value the size of the given type as Little Endian
         *
         * @returns The value
         *
         * @see readBE() for reading a Big Endian value
         */
        template <typename T> T readLE() {
            const T v = little2sys(*reinterpret_cast<const T *>(this->mData));
            this->mData += sizeof(T);
            return v;
        }

        /** Reads a value the size of the given type as Big Endian
         *
         * @returns The value
         *
         * @see readLE() for reading a Little Endian value
         */
        template <typename T> T readBE() {
            const T v = big2sys(*reinterpret_cast<const T *>(this->mData));
            this->mData += sizeof(T);
            return v;
        }

        /** Reads a value the size of the given type using the given endian/byte
         * order
         *
         * @param endian The byte order to read as
         * @returns The value
         *
         * @see readBE() for reading a Big Endian value
         * @see readLE() for reading a Little Endian value
         */
        template <typename T> T read(const io::ByteOrder endian) {
            T v;
            if (endian == io::ByteOrder::LITTLE)
                v = little2sys(*reinterpret_cast<const T *>(this->mData));
            else
                v = big2sys(*reinterpret_cast<const T *>(this->mData));
            this->mData += sizeof(T);
            return v;
        }

        // writing
        void writeByte(uint8_t v);

        void writeBytes(const uint8_t *v, size_t size);

        void writeSignedByte(int8_t v);
        template <typename T>
        void write(const T v, const io::ByteOrder endian) {
            if (endian == io::ByteOrder::LITTLE)
                *reinterpret_cast<T *>(this->mData) = little2sys(v);
            else
                *reinterpret_cast<T *>(this->mData) = big2sys(v);
            this->mData += sizeof(T);
        }

        template <typename T> void writeLE(const T v) {
            *reinterpret_cast<T *>(this->mData) = little2sys(v);
            this->mData += sizeof(T);
        }

        template <typename T> void writeBE(const T v) {
            *reinterpret_cast<T *>(this->mData) = big2sys(v);
            this->mData += sizeof(T);
        }

        [[nodiscard]] uint8_t *getData() const;

        [[nodiscard]] uint8_t *getDataRelative() const;

        uint8_t *&getDataRef();

        uint8_t *&getDataRefRelative();

        uint8_t *readOfSize(size_t sz);

        std::vector<uint8_t> readOfSizeVec(size_t sz);

        void readInto(uint8_t *into, size_t sz);

        [[nodiscard]] size_t getPosition() const;

        std::string readString(size_t size);

        std::string readStringNullTerminated();

        void writeString(const std::string &input);

        static void trimWString(std::wstring &input);

        std::u16string readWChar2Byte(size_t size, io::ByteOrder endian);

        std::u16string readWChar2ByteNT(io::ByteOrder endian);

        void writeWChar2Byte(const std::u16string &input, io::ByteOrder endian,
                             bool nullTerminate);

        std::u32string readWChar4Byte(size_t size, io::ByteOrder endian);

        std::u32string readWChar4ByteNT(io::ByteOrder endian);

        void writeWChar4Byte(const std::u32string &input, io::ByteOrder endian,
                             bool nullTerminate);

        static LIBLCE_API std::wstring
        u16stringToWstring(const std::u16string &str);

        static LIBLCE_API std::u16string
        wstringToU16string(const std::wstring &str);

        static LIBLCE_API std::wstring
        u32stringToWstring(const std::u32string &str);

        static LIBLCE_API std::string
        u16stringToString(const std::u16string &str);

        static LIBLCE_API std::u16string
        stringToU16String(const std::string &str);
        static LIBLCE_API std::string wstringToString(const std::wstring &str);
        static LIBLCE_API std::wstring stringToWString(const std::string &str);

        template <typename T> constexpr T swapOrder_f(const T input) {
            uint8_t resultBytes[sizeof(T)];
            const uint8_t *ib = reinterpret_cast<const uint8_t *>(&input);
            uint8_t *r = resultBytes + (sizeof(T) - 1);

            for (size_t i = 0; i < sizeof(T); ++i) {
                *r-- = *ib++;
            }

            T result;
            memcpy(&result, resultBytes, sizeof(T));
            return result;
        }

        // this is a mess but theoretically will actually use the byteswap
        // instruction
        template <typename T> constexpr T swapOrder(const T input) {
            if constexpr (std::is_integral_v<T>) {
                if constexpr (sizeof(T) == 2) {
#if defined(__clang__) || defined(__GNUC__)
                    return static_cast<T>(
                        __builtin_bswap16(static_cast<uint16_t>(input)));
#elif defined(_MSC_VER)
                    return static_cast<T>(
                        _byteswap_ushort(static_cast<unsigned short>(input)));
#else
                    return swapOrder_f(input);
#endif
                } else if constexpr (sizeof(T) == 4) {
#if defined(__clang__) || defined(__GNUC__)
                    return static_cast<T>(
                        __builtin_bswap32(static_cast<uint32_t>(input)));
#elif defined(_MSC_VER)
                    return static_cast<T>(
                        _byteswap_ulong(static_cast<unsigned long>(input)));
#else
                    return swapOrder_f(input);
#endif
                } else if constexpr (sizeof(T) == 8) {
#if defined(__clang__) || defined(__GNUC__)
                    return static_cast<T>(
                        __builtin_bswap64(static_cast<uint64_t>(input)));
#elif defined(_MSC_VER)
                    return static_cast<T>(
                        _byteswap_uint64(static_cast<unsigned __int64>(input)));
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

        template <typename T> constexpr T big2sys(const T a) {
#ifdef BR_BIG_ENDIAN
            return a;
#else
            return swapOrder(a);
#endif
        }

        template <typename T>
        constexpr T little2sys(const T a) { // also is working as sys2little
#ifdef BR_BIG_ENDIAN
            return swapOrder(a);
#else
            return a;
#endif
        }

      private:
        uint8_t *mOrigin; /**< Data origin */
        uint8_t *mData;   /**< Data pointer, holds where we are in the array */
    };
} // namespace lce::io

#endif // BINARYIO_H
