//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORFILECOMMONS_H
#define COLORFILECOMMONS_H
#include <cstdint>
#include <optional>
#include <vector>

#include <Color/Color.h>
#include <Filesystem/Filesystem.h>

namespace lce::color {

    class LIBLCE_API ColorFileCommons {
      public:
        virtual ~ColorFileCommons() = default;
        uint32_t version;
        std::vector<Color> colors;
        std::optional<Color> getColorByName(std::string name);

        ColorFileCommons() = default;

        ColorFileCommons(const std::vector<Color> &colors, uint32_t version);

        void addColor(Color color);

        [[nodiscard]] virtual uint8_t *create() const = 0;
        [[nodiscard]] virtual uint64_t getSize() const = 0;
    };

    class LIBLCE_API ColorFile final : public ColorFileCommons {
      public:
        std::vector<WorldColor> worldColors;

        ColorFile();
        ColorFile(std::vector<Color> colors,
                  std::vector<WorldColor> worldColors);

        static ColorFile read(std::vector<uint8_t> data);
        [[nodiscard]] uint64_t getSize() const override;

        [[nodiscard]] uint8_t *create() const override;
    };

    class LIBLCE_API ColorFileOld final : public ColorFileCommons {
      public:
        ColorFileOld();
        explicit ColorFileOld(const std::vector<Color> &colors);
        ColorFileOld(const std::vector<Color> &colors, uint32_t version);

        static ColorFileOld read(std::vector<uint8_t> data);

        [[nodiscard]] uint8_t *create() const override;
        [[nodiscard]] uint64_t getSize() const override;
    };

} // namespace lce::color

#endif // COLORFILECOMMONS_H
