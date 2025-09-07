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

    class LIBLCE_API SaveFile final : public SaveFileCommons {
      public:
        /** Creates a save file */
        explicit SaveFile(io::ByteOrder byteOrder = io::ByteOrder::LITTLE,
                          uint16_t origVersion = 11, uint16_t version = 11);

        /** Creates a save file with the contents of a physical folder */
        explicit SaveFile(const fs::Filesystem &fs,
                          io::ByteOrder byteOrder = io::ByteOrder::LITTLE,
                          uint16_t origVersion = 11, uint16_t version = 11);

        /** Creates a save file from serialized data */
        explicit SaveFile(std::vector<uint8_t> data,
                          io::ByteOrder byteOrder = io::ByteOrder::LITTLE);

        /**  */
        [[nodiscard]] uint8_t *serialize() const override;

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
