//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef SAVEFILECOMMONS_H
#define SAVEFILECOMMONS_H
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

    enum SaveFileVersion : uint16_t {
        PR = 1,
        TU0054,
        TU5,
        TU9,
        TU14,
        TU17,
        TU19,
        TU36,
        TU69
    };

    class LIBLCE_API SaveFileCommons : public fs::Filesystem {
      protected:
        // Create a save file from a root folder
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
        [[nodiscard]] uint64_t getSize() const;

        /** Calculates the offset where the index shall be placed, which is
         * always after the header and all the file data
         *
         * @returns The index offset
         */
        [[nodiscard]] uint32_t calculateIndexOffset() const;

        /** Serializes the save file
         *
         * @returns The serialized save file
         */
        [[nodiscard]] virtual uint8_t *serialize() const = 0;

        /** @returns The save file without requiring endian and version */
        static SaveFileCommons *
        deserializeAuto(const std::vector<uint8_t> &data);
        /** @returns The serialized save file's version */
        static uint16_t
        getVersionFromData(std::vector<uint8_t> data,
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
        static constexpr uint32_t HEADER_SIZE = 12;
        // NOTE: this could be wrong name
        uint16_t mOriginalVersion;
        uint16_t mVersion;
        io::ByteOrder mByteOrder;
    };
} // namespace lce::save

#endif // SAVEFILECOMMONS_H
