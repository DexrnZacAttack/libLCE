//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef OLDSAVEFILE_H
#define OLDSAVEFILE_H

#include <Save/SaveFileCommons.h>

namespace lce::save {
    class LIBLCE_API SaveFileOld : public SaveFileCommons {
      public:
        explicit SaveFileOld(
            std::vector<uint8_t> data,
            io::ByteOrder byteOrder =
                io::ByteOrder::BIG); // big endian cuz xb360 was the only
                                     // edition with this format

        /** Creates an old format save file */
        explicit SaveFileOld(io::ByteOrder byteOrder = io::ByteOrder::BIG,
                             uint16_t origVersion = PR, uint16_t version = PR);

        /** Creates an old format save file with the contents of a physical
         * folder
         */
        explicit SaveFileOld(const fs::Filesystem &fs,
                             io::ByteOrder byteOrder = io::ByteOrder::BIG,
                             uint16_t origVersion = PR, uint16_t version = PR);

        SaveFileCommons *migrateVersion(uint16_t version) override;

        [[nodiscard]] uint8_t *serialize() const override;

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
