/** @file LzxCompression.cpp
 *
 * @author DexrnZacAttack
 * @date 3/16/26
 * 
 * @device zPc-i2
 * @license lzxdecoder.LICENSE
 */
#include "LCE/compression/LzxCompression.h"

#include <array>
#include <cstring>

namespace lce {
    namespace compression {
        std::array<std::uint8_t, 52> LzxCompression::Decompressor::s_extraBits = LzxCompression::Decompressor::extraBitsInit();
        std::array<std::uint32_t, 51> LzxCompression::Decompressor::s_positionBase = LzxCompression::Decompressor::positionBaseInit();

        LzxCompression::Decompressor::Decompressor(const int windowBits) {
            std::uint32_t windowSize = 1 << windowBits;
            int posnSlots;

            // todo throw exception for window size range!!!

            // this->m_state.actualSize = 0;
            this->m_state.window = new std::uint8_t[windowSize];
            memset(this->m_state.window, 0xDC, sizeof(windowSize));

            this->m_state.actualSize = windowSize;
            this->m_state.windowSize = windowSize;
            this->m_state.windowPosition = 0;

            // init of statics moved to their own init functions which run at start time

            switch (windowBits) {
            case 20:
                posnSlots = 42;
                break;
            case 21:
                posnSlots = 50;
                break;
            default:
                posnSlots = windowBits << 1;
                break;
            }

            this->m_state.r0 = 1;
            this->m_state.r1 = 1;
            this->m_state.r2 = 1;

            this->m_state.mainElementsCount = NUM_CHARS + (posnSlots << 3);
            this->m_state.headerRead = 0;
            this->m_state.framesRead = 0;
            this->m_state.blockRemaining = 0;
            this->m_state.blockType = BlockType::INVALID;

            this->m_state.intelCurrentPosition = 0;
            this->m_state.intelStarted = 0;

            //can skip 0-init because std::array already handles it
        }

        int LzxCompression::Decompressor::decompress(bio::io::interface::IReadable &input, std::size_t inputLength,
            bio::io::interface::IWritable &output, std::size_t outputLength) {
            // todo bitbuffer then this
        }

        std::array<std::uint8_t, 52> LzxCompression::Decompressor::extraBitsInit() {
            std::array<std::uint8_t, 52> bits;

            for (int i = 0, j = 0; i <= 50; i += 2) {
                bits[i + 1] = j;
                bits[i] = j;

                if (i != 0 && j < 17) {
                    j++;
                }
            }

            return bits;
        }

        std::array<std::uint32_t, 51> LzxCompression::Decompressor::positionBaseInit() {
            std::array<std::uint32_t, 51> pos;

            for (int i = 0, j = 0; i <= 50; i++) {
                pos[i] = j;

                j += 1 << s_extraBits[i];
            }

            return pos;
        }
    } // compression
} // lce