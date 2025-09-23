//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORS_H
#define COLORS_H
#include "IO/Serializable.h"

#include <libLCE.h>
#include <string>

#include <IO/BinaryIO.h>

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
        std::string name;

      protected:
        ColorCommons();
        explicit ColorCommons(std::string name);
    };

    /** Contains a color, used for multiple things in-game. */
    class LIBLCE_API Color final : public ColorCommons {
      public:
        ARGB color;

        Color(std::string name, ARGB color);

        static Color read(std::vector<uint8_t> data);
        static Color read(io::BinaryIO &io);

        std::uint8_t *serialize() const override;
        size_t getSize() const override;
    };

    /** Used for theming the world, typically applied to biomes. */
    class LIBLCE_API WorldColor final : public ColorCommons {
      public:
        ARGB waterColor;      /**< Water color */
        ARGB underwaterColor; /**< Underwater fog color */
        ARGB fogColor;        /**< Fog color */

        WorldColor(std::string name, ARGB waterColor, ARGB underwaterColor,
                   ARGB fogColor);

        // TODO: constructor
        static WorldColor read(std::vector<uint8_t> data);
        static WorldColor read(io::BinaryIO &io);

        uint8_t *serialize() const override;
        size_t getSize() const override;
    };

} // namespace lce::color

#endif // COLORS_H
