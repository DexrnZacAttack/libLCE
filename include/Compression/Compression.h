//
// Created by DexrnZacAttack on 12/28/2024.
//

#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <vector>

#include <IO/BinaryIO.h>
#include <libLCE.h>

namespace lce::compression {
    /** Compression methods
     *
     * @see Type for supported compression types
     */
    class LIBLCE_API Compression {
      public:
        /** Compression types
         *
         * @see https://team-lodestone.github.io/Documentation/LCE/Basic%20Info
         */
        enum class LIBLCE_API Type {
            /** [zlib](https://www.zlib.net/) **/
            ZLIB,
            /** [LZX](https://github.com/FNA-XNA/FNA/blob/master/src/Content/LzxDecoder.cs)
             *
             * Only used on Xbox 360 Edition
             */
            LZX,
            /** Propriatary RLE only used for modern region files (split saves)
             **/
            SPLIT_SAVE,
            /** Propriatary RLE only used for compressing chunks (which usually
               have 2 layers of compression not including region & save file
               compression)
             */
            CHUNK,
            /** Propriatary RLE only used on PlayStation Vita Edition */
            VITA,
            /** [Deflate](https://en.wikipedia.org/wiki/Deflate)
             *
             * Only used on PlayStation 3 Edition.
             */
            DEFLATE,
        };

        /** Decompresses zlib
         *
         * @param in Input vector
         * @param out Output vector
         *
         * @see Compression::Type::ZLIB
         */
        static bool decompressZlib(std::vector<uint8_t> &in,
                                   std::vector<uint8_t> &out);

        /** Decompresses zlib (with a set buffer size and offset)
         *
         * @param in Input vector
         * @param out Output vector
         * @param outSize Output buffer size
         * @param offset Input offset
         *
         * @see Compression::Type::ZLIB
         */
        static bool decompressZlibWithLength(std::vector<uint8_t> &in,
                                             std::vector<uint8_t> &out,
                                             uint32_t outSize, uint32_t offset);

        /** Decompresses chunk-specific RLE
         *
         * @param in Input vector
         * @param out Output vector
         *
         * @see Compression::Type::CHUNK
         */
        static bool decompressChunk(std::vector<uint8_t> &in,
                                    std::vector<uint8_t> &out);

        /** Decompresses PlayStation Vita specific RLE
         *
         * @param in Input vector
         * @param out Output vector
         * @param outSize Output buffer size
         * @param offset Input offset
         *
         * @see Compression::Type::VITA
         */
        static bool decompressVita(std::vector<uint8_t> &in,
                                   std::vector<uint8_t> &out, uint32_t outSize,
                                   uint32_t offset = 0);

        /**
         * Decompresses based on given type
         *
         * @param in Input vector
         * @param out Output vector
         * @param type Compression type
         */
        static bool decompress(std::vector<uint8_t> &in,
                               std::vector<uint8_t> &out, Type type);

        /** @returns The compressed save file's size */
        static uint32_t getCompressedSaveFileSize(std::vector<uint8_t> &in,
                                                  io::ByteOrder byteOrder);
    };

} // namespace lce::compression

#endif // COMPRESSION_H
