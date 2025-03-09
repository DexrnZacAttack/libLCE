//
// Created by DexrnZacAttack on 12/21/2024.
//

#include "SaveFileOld.h"

#include <chrono>
#include "IndexInnerFile.h"
#include "SaveFile.h"
#include "../io/BinaryIO.h"

namespace lce::save {
    SaveFileOld::SaveFileOld(ByteOrder endian) {
        this->endian = endian;
    }

    /**
     * Reads an old-format save file from a pointer to the data
     * @param data The data you want to read (a save file)
     * @param endian The endian/byte order of the save file (default Little)
     * @return The old-format save file.
     */
    SaveFileOld::SaveFileOld(std::vector<uint8_t> data, ByteOrder endian) {
        this->endian = endian;

        io::BinaryIO io(data.data());
        uint32_t indexSize = this->SaveFileOld::getIndexEntrySize();

        this->indexOffset = io.read<uint32_t>(this->endian);

        if (this->indexOffset > data.size())
            throw std::runtime_error("Index offset points to an area out of bounds of the data given.");

        this->indexFileCount = io.read<uint32_t>(this->endian) / indexSize;
        this->originalVersion = io.read<uint16_t>(this->endian);
        this->version = io.read<uint16_t>(this->endian);

        std::vector<IndexInnerFile> index(this->indexFileCount);

        for (int i = 0; i < this->indexFileCount; ++i) {
            io.seek(this->indexOffset + (indexSize * i));
            // read the index entry
            IndexInnerFile inf = IndexInnerFile::IndexInnerFile(io.readOfSize(indexSize), true, this->endian);
            // read the data, maybe should be changed
            io.seek(inf.offset);
            inf.data = new uint8_t[inf.size];
            io.readInto(inf.data, inf.size);
            // set the entry
            index[i] = inf;
        }

        this->index = index;
    }

    /**
     * Writes the save file
     * @return Pointer to the save file
     */
    const uint8_t *SaveFileOld::create() {
        io::BinaryIO io(this->getSize());

        this->indexOffset = HEADER_SIZE;

        io.write<uint32_t>(0, this->endian);
        io.write<uint32_t>(this->indexFileCount * this->getIndexEntrySize(), this->endian);
        io.write<uint16_t>(this->originalVersion, this->endian);
        io.write<uint16_t>(this->version, this->endian);

        for (const auto& file: this->index) {
            file.offset = io.getPosition();
            io.writeBytes(file.data, file.size);
            indexOffset += file.size;
        }

        for (const auto& file: this->index) {
            io.writeWChar2Byte(file.name, this->endian);
            io.write<uint32_t>(file.size, this->endian);
            io.write<uint32_t>(file.offset, this->endian);
        }

        io.seek(0);
        io.write<uint32_t>(this->indexOffset, this->endian);

        return io.getData();
    }

    /**
     * Migrates from SaveFileOld to SaveFile
     * @param version The version you want to migrate to (has no effect except for changing the version in the header)
     * @return The new SaveFile
     */
    SaveFile *SaveFileOld::migrate(uint16_t version = 2) {
        if (version <= 1)
            throw std::invalid_argument("Version must be greater than 1. (otherwise it's not really a migration now is it?)");

        const uint32_t indexOffset = this->indexOffset;
        const uint32_t indexFileCount = this->indexFileCount / 136; // new format

        uint16_t originalVersion;
        if (version > 3)
            originalVersion = this->version;
        else
            originalVersion = 0;

        std::vector<IndexInnerFile> index(indexFileCount);

        for (const auto& file : this->index) {
            auto updated = file;
            updated.timestamp = 0; // LCE does this
            index.push_back(updated);
        }

        return new SaveFile(indexOffset, indexFileCount, originalVersion, version, index);
    }

    /**
     * Gets the size of an index entry based on the save file class type.
     * @return The size of an index entry
     */
    uint32_t SaveFileOld::getIndexEntrySize() {
        return 136;
    }

}