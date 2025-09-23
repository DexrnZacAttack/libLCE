//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <cstdint>
#include <string>
#include <vector>

#include <IO/BinaryIO.h>
#include <Save/SaveFileCommons.h>

namespace lce::save {
    /**
     * LCE SaveGame (savegame.dat) File
     * \brief A standard archive that can store files,
     * used for storing the world (players, regions, etc).
     *
     * @see SaveFileOld for the pre-release variant
     * @see
     * https://team-lodestone.github.io/Documentation/LCE/Saving/Save%20Format
     *
     * ## Header
     * - Index Offset (`uint32_t`)
     *   - Determines where to seek to start reading the `File Index`
     *   - Always stored at the end of the file
     * - File Count (`uint32_t`)
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
     * - Modified timestamp (`uint64_t`)
     *   - Usually unusable due to the console returning a propriatary or
     * relative timestamp
     */
    class LIBLCE_API SaveFile final : public SaveFileCommons {
      public:
        /** Creates a save file */
        explicit SaveFile(io::ByteOrder byteOrder = io::ByteOrder::LITTLE,
                          uint16_t origVersion = 11, uint16_t version = 11);

        /** Creates a save file with the contents of a physical folder */
        explicit SaveFile(const Filesystem &fs,
                          io::ByteOrder byteOrder = io::ByteOrder::LITTLE,
                          uint16_t origVersion = 11, uint16_t version = 11);

        /** Creates a save file from serialized data */
        explicit SaveFile(std::vector<uint8_t> data,
                          io::ByteOrder byteOrder = io::ByteOrder::LITTLE);

        uint8_t *serialize() const override;

      protected:
        /**
         * Gets the size of an index entry based on the save file class type.
         * @return The size of an index entry
         */
        [[nodiscard]] uint32_t getIndexEntrySize() const override {
            return 144;
        }

      public:
        SaveFileCommons *migrateVersion(uint16_t version) override;

      protected:
        ;
    };

} // namespace lce::save

#endif // SAVEFILE_H
