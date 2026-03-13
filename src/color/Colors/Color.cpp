//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "LCE/color/Color.h"
#include "LCE/color/ColorFile.h"

namespace lce::color {
    Color::Color(const ARGB color) : color(color) {}

    Color::Color(std::vector<uint8_t> &data)
        : Color(bio::buffer::BinaryBuffer(data.data())) {}

    Color::Color(uint8_t *data) : Color(bio::buffer::BinaryBuffer(data)) {}

    Color::Color(bio::buffer::BinaryBuffer &&io) : Color(io) {}

    Color::Color(bio::buffer::BinaryBuffer &io) { this->color = io.readLE<ARGB>(); }

    size_t Color::getSize() const { return sizeof(ARGB); }

    uint8_t *Color::serialize() const {
        bio::buffer::BinaryBuffer io(getSize());

        io.writeLE<ARGB>(color);

        return io.begin();
    }

    ColorCommons::ColorCommons() {}

} // namespace lce::color
