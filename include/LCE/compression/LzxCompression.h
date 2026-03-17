/** @file LzxCompression.h
 *
 * @author DexrnZacAttack
 * @date 3/16/26
 * 
 * @device zPc-i2
 * @license lzxdecoder.LICENSE
 */
#ifndef LCE_LZXCOMPRESSION_H
#define LCE_LZXCOMPRESSION_H

#include "BinaryIO/container/FixedArray.h"
#include "BinaryIO/io/interface/IReadable.h"

#include <array>
#include <cstdint>
#include <optional>

namespace lce::compression {
    /** Provides compression and decompression for LZX, the primary compression method used in Xbox 360 Edition.
     *
     * A port of @link https://github.com/FNA-XNA/FNA/blob/master/src/Content/LzxDecoder.cs @endlink
     *
     * Dual licensed under the LGPL and MSPL.
     *
     * @ref lzxdecoder.LICENSE "See lzxdecoder.LICENSE for licensing info"
     */
    class LzxCompression {
    public:
        enum BlockType {
            INVALID = 0,
            VERBATIM = 1,
            ALIGNED = 2,
            UNCOMPRESSED = 3
        };

        static constexpr std::uint16_t MIN_MATCH = 2;
        static constexpr std::uint16_t MAX_MATCH = 257;
        static constexpr std::uint16_t NUM_CHARS = 256;
        static constexpr std::uint16_t PRETREE_NUM_ELEMENTS = 20;
        static constexpr std::uint16_t ALIGNED_NUM_ELEMENTS = 8;
        static constexpr std::uint16_t NUM_PRIMARY_LENGTHS = 7;
        static constexpr std::uint16_t NUM_SECONDARY_LENGTHS = 249;

        static constexpr std::uint16_t PRETREE_MAXSYMBOLS = PRETREE_NUM_ELEMENTS;
        static constexpr std::uint16_t PRETREE_TABLEBITS = 6;
        static constexpr std::uint16_t MAINTREE_MAXSYMBOLS = NUM_CHARS + 50 * 8;
        static constexpr std::uint16_t MAINTREE_TABLEBITS = 12;
        static constexpr std::uint16_t LENGTH_MAXSYMBOLS = NUM_SECONDARY_LENGTHS + 1;
        static constexpr std::uint16_t LENGTH_TABLEBITS = 12;
        static constexpr std::uint16_t ALIGNED_MAXSYMBOLS = ALIGNED_NUM_ELEMENTS;
        static constexpr std::uint16_t ALIGNED_TABLEBITS = 7;

        static constexpr std::uint16_t LENTABLE_SAFETY = 64;

        struct State {
            std::uint32_t r0, r1, r2;           /**< For the LRU offset system */
            std::uint16_t mainElementsCount;    /**< Number of main tree elements */
            // todo boolean????
            std::int32_t  headerRead;           /**< Have we started decoding at all yet? */
            // todo I wonder if we can turn this into a Block struct
            BlockType     blockType;            /**< Type of this block */
            std::uint32_t blockLength;          /**< Uncompressed length of this block */
            std::uint32_t blockRemaining;       /**< Uncompressed bytes still left to decode */
            std::uint32_t framesRead;           /**< The number of CFDATA blocks processed */
            std::int32_t  intelFilesize;        /**< Magic header value used for transform */
            std::int32_t  intelCurrentPosition; /**< Current offset in transform space */
            std::int32_t  intelStarted;         /**< Have we seen any translatable data yet? */

            std::array<std::uint16_t, (1 << PRETREE_TABLEBITS) + (PRETREE_MAXSYMBOLS << 1)> pretreeTable;
            std::array<std::uint8_t, PRETREE_MAXSYMBOLS + LENTABLE_SAFETY> pretreeLength;
            std::array<std::uint16_t, (1 << MAINTREE_TABLEBITS) + (MAINTREE_MAXSYMBOLS << 1)> maintreeTable;
            std::array<std::uint8_t, MAINTREE_MAXSYMBOLS + LENTABLE_SAFETY> maintreeLen;
            std::array<std::uint16_t, (1 << LENGTH_TABLEBITS) + (LENGTH_MAXSYMBOLS << 1)> lengthTable;
            std::array<std::uint8_t, LENGTH_MAXSYMBOLS + LENTABLE_SAFETY> lengthLength;
            std::array<std::uint16_t, (1 << ALIGNED_TABLEBITS) + (ALIGNED_MAXSYMBOLS << 1)> alignedTable;
            std::array<std::uint8_t, ALIGNED_MAXSYMBOLS + LENTABLE_SAFETY> alignedLength;

            /* NEEDED MEMBERS
             * CAB actualsize
             * CAB window
             * CAB window_size
             * CAB window_posn
             */
            std::uint32_t actualSize;
            std::uint8_t *window;
            std::uint32_t windowSize;
            std::uint32_t windowPosition;
        };

        class Decompressor {
        public:
            static std::array<std::uint8_t, 52> s_extraBits;
            static std::array<std::uint32_t, 51> s_positionBase;

            Decompressor(int windowBits);

            int decompress(bio::io::interface::IReadable &input, std::size_t inputLength, bio::io::interface::IWritable &output,
                           std::size_t outputLength);

        private:
            static std::array<std::uint8_t, 52> extraBitsInit();

            static std::array<std::uint32_t, 51> positionBaseInit();

            State m_state;
        };
    };
}

#endif // LCE_LZXCOMPRESSION_H