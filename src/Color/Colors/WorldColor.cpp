//
// Created by DexrnZacAttack on 1/2/2025.
//

#include <Color/Color.h>

namespace lce::color {
    WorldColor::WorldColor(const ARGB waterColor, const ARGB underwaterColor,
                           const ARGB fogColor)
        : waterColor(waterColor), underwaterColor(underwaterColor),
          fogColor(fogColor) {}

    WorldColor::WorldColor(std::vector<uint8_t> &data)
        : WorldColor(io::BinaryIO(data.data())) {}

    WorldColor::WorldColor(uint8_t *data) : WorldColor(io::BinaryIO(data)) {}

    WorldColor::WorldColor(io::BinaryIO &&io) : WorldColor(io) {}

    WorldColor::WorldColor(io::BinaryIO &io) {
        this->waterColor = io.readLE<ARGB>();
        this->underwaterColor = io.readLE<ARGB>();
        this->fogColor = io.readLE<ARGB>();
    }

    uint8_t *WorldColor::serialize() const {
        io::BinaryIO io(getSize());

        io.writeLE<ARGB>(waterColor);
        io.writeLE<ARGB>(underwaterColor);
        io.writeLE<ARGB>(fogColor);

        return io.getData();
    }

    size_t WorldColor::getSize() const { return sizeof(ARGB) * 3; }
} // namespace lce::color
