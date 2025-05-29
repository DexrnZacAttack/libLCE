//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORS_H
#define COLORS_H
#include <libLCE.h>
#include <string>

#include <IO/BinaryIO.h>

namespace lce::color {

    struct ARGB {
        uint8_t a;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    class LIBLCE_API ColorCommons {
    public:
        virtual ~ColorCommons() = default;
        std::string name;

        ColorCommons();
        explicit ColorCommons(std::string name);
        virtual uint32_t getSize();
    };

    class LIBLCE_API Color final : public ColorCommons {
    public:
        ARGB color;

        Color(std::string name, ARGB color);
        static Color read(std::vector<uint8_t> data);

        static Color read(io::BinaryIO& io);
        uint8_t* create();
        uint32_t getSize() override;
    };

    class LIBLCE_API WorldColor final : public ColorCommons {
    public:
        ARGB waterColor;
        ARGB underwaterColor;
        ARGB fogColor;

        WorldColor(std::string name, ARGB waterColor, ARGB underwaterColor, ARGB fogColor);

        static WorldColor read(std::vector<uint8_t> data);
        static WorldColor read(io::BinaryIO& io);

        uint8_t* create();
        uint32_t getSize() override;
    };

} // namespace lce::color

#endif // COLORS_H
