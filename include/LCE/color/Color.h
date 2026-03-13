//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORS_H
#define COLORS_H
#include "LCE/io/Serializable.h"

#include "LCE/libLCE.h"
#include <string>

#include <BinaryIO/buffer/BinaryBuffer.h>

namespace lce::color {
    /** ARGB Color structure */
    struct ARGB {
        uint8_t a; /**< Alpha/Transparency */
        uint8_t r; /**< Red */
        uint8_t g; /**< Green */
        uint8_t b; /**< Blue */
    };

    /** Base Color File (.COL) subtype */
    class LIBLCE_API ColorCommons : public io::Serializable {
      public:
        ~ColorCommons() override = default;

      protected:
        ColorCommons();
    };

    /** Contains a color, used for coloring things in-game. */
    class LIBLCE_API Color final : public ColorCommons {
      public:
        explicit Color(ARGB color);

        explicit Color(uint8_t *data);
        explicit Color(std::vector<uint8_t> &data);
        explicit Color(bio::buffer::BinaryBuffer &&io);
        explicit Color(bio::buffer::BinaryBuffer &io);

        std::uint8_t *serialize() const override;
        size_t getSize() const override;

        ARGB color;
    };

    /** Contains 3 colors used for theming the world, typically applied to
     * biomes. */
    class LIBLCE_API WorldColor final : public ColorCommons {
      public:
        explicit WorldColor(ARGB waterColor, ARGB underwaterColor,
                            ARGB fogColor);

        explicit WorldColor(uint8_t *data);
        explicit WorldColor(std::vector<uint8_t> &data);
        explicit WorldColor(bio::buffer::BinaryBuffer &&io);
        explicit WorldColor(bio::buffer::BinaryBuffer &io);

        uint8_t *serialize() const override;
        size_t getSize() const override;

        ARGB waterColor;      /**< Water color */
        ARGB underwaterColor; /**< Underwater fog color */
        ARGB fogColor;        /**< Fog color */
    };

} // namespace lce::color

#endif // COLORS_H
