//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef OLDSAVEFILE_H
#define OLDSAVEFILE_H

#include <Save/SaveFileCommons.h>

namespace lce::save {
    /**
     * Pre-release LCE SaveGame (savegame.dat) File
     * \brief A standard archive that can store files,
     * used for storing the world (players, regions, etc).
     * Only used on pre-release builds 0035 and below.
     *
     * @see SaveFile for the modern variant
     * @see
     * https://team-lodestone.github.io/Documentation/LCE/Saving/Pre-Release%20Save%20Format
     *
     * ## Header
     * - Index Offset (`uint32_t`)
     *   - Determines where to seek to start reading the `File Index`
     *   - Always stored at the end of the file
     * - Index Size (`uint32_t`)
     *   - `File Index` size in bytes, divide by 136 to get file count.
     * - Original Version (`uint16_t`)
     *   - Should be renamed, seems to be more of a "minimum version" based on
     * the values given by each version.
     * - Version (`uint16_t`)
     *   - Version of the save file
     *
     * @see SaveFileVersion
     *
     * ## File Data
     * For each file,
     * - File Data (`File Size` bytes)
     *
     * ## File Index
     * For each file,
     * - Name (144 bytes)
     *   - Written as 16 bit chars (even on systems where `wchar_t` is 4 bytes)
     * - File Size (`uint32_t`)
     * - File Offset (`uint32_t`)
     *   - Offset to the file data
     */
    class LIBLCE_API SaveFileOld : public SaveFileCommons {
      public:
        explicit SaveFileOld(
            std::vector<uint8_t> data,
            io::ByteOrder byteOrder =
                io::ByteOrder::BIG); // big endian cuz xb360 was the only
                                     // edition with this format

        /** Creates an old format save file */
        explicit SaveFileOld(io::ByteOrder byteOrder = io::ByteOrder::BIG,
                             uint16_t origVersion = B0033, uint16_t version = B0033);

        /** Creates an old format save file with the contents of a physical
         * folder
         */
        explicit SaveFileOld(const Filesystem &fs,
                             io::ByteOrder byteOrder = io::ByteOrder::BIG,
                             uint16_t origVersion = B0033, uint16_t version = B0033);

        SaveFileCommons *migrateVersion(uint16_t version) override;

        uint8_t *serialize() const override;

      protected:
        /**
         * Gets the size of an index entry based on the save file class type.
         * @return The size of an index entry
         */
        [[nodiscard]] uint32_t getIndexEntrySize() const override {
            return 136;
        };
    };
} // namespace lce::save

#endif // OLDSAVEFILE_H
