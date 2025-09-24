//
// Created by DexrnZacAttack on 1/2/2025.
//

#include <Color/Color.h>
#include <Color/ColorFile.h>

namespace lce::color {
    Color::Color(const ARGB color) : color(color) {}

    Color::Color(std::vector<uint8_t> &data)
        : Color(io::BinaryIO(data.data())) {}

    Color::Color(uint8_t *data) : Color(io::BinaryIO(data)) {}

    Color::Color(io::BinaryIO &&io) : Color(io) {}

    Color::Color(io::BinaryIO &io) { this->color = io.readLE<ARGB>(); }

    size_t Color::getSize() const { return sizeof(ARGB); }

    uint8_t *Color::serialize() const {
        io::BinaryIO io(getSize());

        io.writeLE<ARGB>(color);

        return io.getData();
    }

    ColorCommons::ColorCommons() {}

} // namespace lce::color
