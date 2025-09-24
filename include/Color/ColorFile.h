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

    /** Color file versions */
    enum ColorFileVersion : uint32_t {
        OLD, /**< Old format */
        /** New format, contains world colors
         *
         * Used from TU69 onwards
         */
        NEW
    };

    /** LCE Color (.COL) File Base class
     *
     * @see ColorFile
     * @see ColorFileOld
     */
    class LIBLCE_API ColorFileCommons : public io::Serializable {
      public:
        ~ColorFileCommons() override = default;

        ColorFileCommons() = default;

        ColorFileCommons(const std::unordered_map<std::string, Color> &colors,
                         uint32_t version);

        static ColorFileCommons *deserializeAuto(uint8_t *data);
        static ColorFileCommons *deserializeAuto(std::vector<uint8_t> &data) {
            return deserializeAuto(data.data());
        };

        void addColor(const std::string &name, Color color);

        uint32_t getVersion() const;

        Color *getColor(const std::string &name);

        std::unordered_map<std::string, Color> &getColors();

      protected:
        uint32_t version;
        std::unordered_map<std::string, Color> colors;
    };

    /**
     * LCE Color (.COL) File
     * \brief A file that stores data driven colors used in-game for things such
     * as coloring water, fog, the sky, etc.
     *
     * @see ColorFileOld
     * @see https://team-lodestone.github.io/Documentation/LCE/File%20Types/COL
     *
     * ## Header
     * - Version (`uint32_t`)
     *
     * ## Color Index
     * - Color Count (`uint32_t`)
     *     * For each color,
     * - Name Size (`uint16_t`)
     * - Name (`Name Size` bytes)
     *
     * - ARGB Color (4 bytes)
     *
     * ## World Color Index
     * - World Color Count (`uint32_t`)
     *
     * For each world color,
     * - Name Size (`uint16_t`)
     * - Name (`Name Size` bytes)
     *
     * - Water Color (4 bytes)
     * - Underwater Fog Color (4 bytes)
     * - Fog Color (4 bytes)
     */
    class LIBLCE_API ColorFile final : public ColorFileCommons {
      public:
        ColorFile() = default;

        ColorFile(
            const std::unordered_map<std::string, Color> &colors,
            const std::unordered_map<std::string, WorldColor> &worldColors)
            : ColorFileCommons(colors, 4), worldColors(worldColors) {}

        explicit ColorFile(uint8_t *data) : ColorFile(io::BinaryIO(data)) {}
        explicit ColorFile(std::vector<uint8_t> &data)
            : ColorFile(io::BinaryIO(data.data())) {}
        explicit ColorFile(io::BinaryIO &&io) : ColorFile(io) {};
        explicit ColorFile(io::BinaryIO &io);

        void addWorldColor(const std::string &name, WorldColor color);

        WorldColor *getWorldColor(const std::string &name);

        std::unordered_map<std::string, WorldColor> &getWorldColors();

        size_t getSize() const override;
        uint8_t *serialize() const override;

      protected:
        std::unordered_map<std::string, WorldColor> worldColors;
    };

    /**
     * LCE Color (.COL) File
     * \brief A file that stores data driven colors used in-game.
     *
     * @see ColorFile
     * @see https://team-lodestone.github.io/Documentation/LCE/File%20Types/COL
     *
     * ## Header
     * - Version (`uint32_t`)
     *
     * ## Color Index
     * - Color Count (`uint32_t`)
     *     * For each color,
     * - Name Size (`uint16_t`)
     * - Name (`Name Size` bytes)
     *
     * - ARGB Color (4 bytes)
     */
    class LIBLCE_API ColorFileOld final : public ColorFileCommons {
      public:
        ColorFileOld() = default;

        explicit ColorFileOld(
            const std::unordered_map<std::string, Color> &colors);
        ColorFileOld(const std::unordered_map<std::string, Color> &colors,
                     uint32_t version);

        explicit ColorFileOld(uint8_t *data)
            : ColorFileOld(io::BinaryIO(data)) {}
        explicit ColorFileOld(std::vector<uint8_t> &data)
            : ColorFileOld(io::BinaryIO(data.data())) {}

        explicit ColorFileOld(io::BinaryIO &&io) : ColorFileOld(io) {};
        explicit ColorFileOld(io::BinaryIO &io);

        uint8_t *serialize() const override;
        size_t getSize() const override;
    };

} // namespace lce::color

#endif // COLORFILECOMMONS_H
