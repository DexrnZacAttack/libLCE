//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef SAVEFILECOMMONS_H
#define SAVEFILECOMMONS_H
#include "IO/Serializable.h"

#include <Filesystem/Filesystem.h>
#include <IO/BinaryIO.h>
#include <libLCE.h>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <Filesystem/Directory.h>
#include <Filesystem/File.h>

namespace lce::save {
    class SaveFileOld;
    class SaveFile;

    /** Save file versions, each value's name denotes the first TU it was used
     * in
     *
     * @see
     * https://team-lodestone.github.io/Documentation/LCE/Saving/LCE%20Save%20File%20Versions
     * for more info
     */
    enum SaveFileVersion : uint16_t {
        B0033 = 1, /**< Pre-release builds, Builds 0033-0035 */
        B0054,     /**< Used between pre-release build 0054 and TU 4 */
        TU5,       /**< Used between TUs 5-8  */
        TU9,       /**< Used between TUs 9-13  */
        TU14,      /**< Used between TUs 14-16  */
        TU17,      /**< Used between TUs 17-18  */
        TU19,      /**< Used between TUs 19-35  */
        TU36,      /**< Used between TUs 36-68  */
        TU69       /**< Used on TU 69 onwards  */
    };

    /** Base class for the 2 different Save File types
     *
     * @see SaveFile
     * @see SaveFileOld
     */
    class LIBLCE_API SaveFileCommons : public fs::Filesystem,
                                       public io::Serializable {
      protected:
        /** Creates a save file with the contents of the given Filesystem */
        SaveFileCommons(const Filesystem &fs, const io::ByteOrder byteOrder,
                        const uint16_t origVersion = 11,
                        const uint16_t version = 11)
            : Filesystem(fs), mOriginalVersion(origVersion), mVersion(version),
              mByteOrder(byteOrder) {};

        explicit SaveFileCommons(
            const io::ByteOrder byteOrder = io::ByteOrder::LITTLE,
            const uint16_t origVersion = 11, const uint16_t version = 11)
            : mOriginalVersion(origVersion), mVersion(version),
              mByteOrder(byteOrder) {};

        /**
         * Gets the size of an index entry based on the save file class type.
         * @return The size of an index entry
         */
        [[nodiscard]] virtual uint32_t getIndexEntrySize() const { return 0; };

      public:
        /** Calculates the total size of the save file in bytes
         *
         * @returns The save file size in bytes
         */
        size_t getSize() const override;

        /** Calculates the offset where the index shall be placed, which is
         * always after the header and all the file data
         *
         * @returns The index offset
         */
        [[nodiscard]] uint32_t calculateIndexOffset() const;

        /** @returns The save file without requiring endian and version */
        static SaveFileCommons *deserializeAuto(std::vector<uint8_t> &data);

        /** @returns The serialized save file's version */
        static uint16_t
        getVersionFromData(std::vector<uint8_t> &data,
                           io::ByteOrder byteOrder = io::ByteOrder::LITTLE);

        /** @returns The save file's original version */
        [[nodiscard]] uint16_t getOriginalVersion() const;
        /** @returns The save file's version */
        [[nodiscard]] uint16_t getVersion() const;
        /** @returns The save file's byte order (endianness) */
        [[nodiscard]] io::ByteOrder getByteOrder() const;
        /** Sets the save file's original version */
        void setOriginalVersion(uint16_t version);
        /** Sets the save file's version
         *
         * NOTE: please use #migrateVersion if you want to upgrade the file
         * to the new format (SOON TO CHANGE)
         */
        void setVersion(uint16_t version);
        /** Sets the save file's byte order (endianness) */
        void setEndian(io::ByteOrder byteOrder);

        /** Migrates the current save file to the chosen version should there be
         * any differences */
        virtual SaveFileCommons *migrateVersion(uint16_t version) = 0;

        /** Detects the save file's byte order based off of serialized data
         *
         * @returns The save file's byte order (endianness)
         */
        static io::ByteOrder detectByteOrder(std::vector<uint8_t> data);

        friend std::wostream &operator<<(std::wostream &wos,
                                         const SaveFileCommons &f) {
            wos << L"SaveFileCommons [" << L"indexOffset="
                << f.calculateIndexOffset() << L", fileCount="
                << f.getRoot()->getFileCount() << L", version="
                << f.getVersion() << L", originalVersion="
                << f.getOriginalVersion() << L"]";
            return wos;
        }

      protected:
        /** Header size in bytes of the file once serialized
         * - Index Offset (`uint32_t`) -> 4 bytes
         * - File Count (`uint32_t`) -> 4 bytes
         * - Original Version (`uint16_t`) -> 2 bytes
         * - Version (`uint16_t`) -> 2 bytes
         */
        static constexpr uint32_t HEADER_SIZE = 12;
        // NOTE: this could be wrong name
        uint16_t mOriginalVersion;
        uint16_t mVersion;
        io::ByteOrder mByteOrder;
    };
} // namespace lce::save

#endif // SAVEFILECOMMONS_H
