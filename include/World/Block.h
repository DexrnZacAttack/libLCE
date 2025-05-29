//
// Created by DexrnZacAttack on 3/27/2025.
//

#ifndef BLOCK_H
#define BLOCK_H
#include <cstdint>

#include <IO/BinaryIO.h>
#include <libLCE.h>

namespace lce::world {

    class LIBLCE_API Block {
    protected:
        bool waterlogged;
        uint16_t id;
        uint8_t data;

    public:
        Block(uint16_t id, uint8_t data, bool waterlogged);
        Block(uint16_t packed, io::ByteOrder endian = io::ByteOrder::LITTLE);

        uint16_t getPacked() const;

        uint16_t getId() const;
        uint8_t getData() const;
        bool isWaterlogged() const;
    };

} // namespace lce::world

#endif // BLOCK_H
