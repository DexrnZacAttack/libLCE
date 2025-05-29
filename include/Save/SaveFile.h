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

    class LIBLCE_API SaveFile : public SaveFileCommons {
    public:
        SaveFile(uint32_t indexFileCount, uint16_t origVersion, uint16_t version);
        SaveFile();
        explicit SaveFile(io::ByteOrder endian);

        explicit SaveFile(std::vector<uint8_t> data, io::ByteOrder endian = io::ByteOrder::LITTLE);

        [[nodiscard]] uint8_t* toData() const override;

    protected:
        /**
         * Gets the size of an index entry based on the save file class type.
         * @return The size of an index entry
         */
        [[nodiscard]] uint32_t getIndexEntrySize() const override { return 144; };
    };

} // namespace lce::save

#endif // SAVEFILE_H
