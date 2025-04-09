//
// Created by DexrnZacAttack on 3/27/2025.
//

#include "Block.h"

namespace lce::world {
    Block::Block(uint16_t id, uint8_t data, bool waterlogged): id(id), data(data), waterlogged(waterlogged) {
    }

    // UNTESTED
    Block::Block(uint16_t packed, ByteOrder endian) {
        if (endian == BIG) {
            const uint8_t lBlk = (packed & 0xF0) >> 4;
            const uint8_t uBlk = (packed >> 8) & 0x7F;
            this->id = (uBlk << 4) | lBlk;
            this->data = (packed & 0xF0) >> 4;
            this->waterlogged = (packed >> 8) & 0x1;
        } else {
            this->id = (packed >> 4) & 0x7FF;
            this->data = packed & 0xF;
            this->waterlogged = (packed >> 15) & 0x1;
        }
    }

    // todo
    uint16_t Block::getPacked() const {
        return 0;
    }

    uint16_t Block::getId() const {
        return this->id;
    }

    uint8_t Block::getData() const {
        return this->data;
    }

    bool Block::isWaterlogged() const {
        return this->waterlogged;
    }
} // lce