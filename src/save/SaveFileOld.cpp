//
// Created by DexrnZacAttack on 12/21/2024.
//

#include "SaveFileOld.h"

#include <chrono>
#include "IndexInnerFile.h"
#include "SaveFile.h"
#include "../io/BinaryIO.h"

namespace lce::save {
    /**
     * Reads an old-format save file from a pointer to the data
     * @param data The data you want to read (a save file)
     * @return The old-format save file.
     */
    SaveFileOld SaveFileOld::read(std::vector<uint8_t> data) {
        SaveFileOld sf;
        io::BinaryIO io(data.data());
        uint32_t indexSize = sf.getIndexEntrySize();

        sf.indexOffset = io.readB<uint32_t>();
        sf.indexFileCount = io.readB<uint32_t>() / indexSize;
        sf.originalVersion = io.readB<uint16_t>();
        sf.version = io.readB<uint16_t>();

        std::vector<IndexInnerFile> index(sf.indexFileCount);

        for (int i = 0; i < sf.indexFileCount; ++i) {
            io.seek(sf.indexOffset + (indexSize * i));
            // read the index entry
            IndexInnerFile inf = IndexInnerFile::readOld(io.readOfSize(indexSize));
            // read the data, maybe should be changed
            io.seek(inf.offset);
            inf.data = new uint8_t[inf.size];
            io.readInto(inf.data, inf.size);
            // set the entry
            index[i] = inf;
        }

        sf.index = index;

        return sf;
    }

    /**
     * Writes the save file
     * @return Pointer to the save file
     */
    const uint8_t *SaveFileOld::create() {
        const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        io::BinaryIO io(data);

        this->indexOffset = HEADER_SIZE;

        io.writeB<uint32_t>(0);
        io.writeB<uint32_t>(this->indexFileCount * this->getIndexEntrySize());
        io.writeB<uint16_t>(this->originalVersion);
        io.writeB<uint16_t>(this->version);

        for (const auto& file: this->index) {
            file.offset = io.getPosition();
            io.writeBytes(file.data, file.size);
            indexOffset += file.size;
        }

        for (const auto& file: this->index) {
            io.writeWChar2ByteB(file.name);
            io.writeB<uint32_t>(file.size);
            io.writeB<uint32_t>(file.offset);
        }

        io.seek(0);
        io.writeB<uint32_t>(this->indexOffset);

        return io.getData();
    }

    /**
     * Migrates from SaveFileOld to SaveFile
     * @param version The version you want to migrate to (has no effect except for changing the version in the header)
     * @return The new SaveFile
     */
    SaveFile SaveFileOld::migrate(uint16_t version = 2) {
        SaveFile sf;

        if (version <= 1)
            throw std::invalid_argument("Version must be greater than 1. (otherwise it's not really a migration now is it?)");

        sf.indexOffset = this->indexOffset;
        sf.indexFileCount = this->indexFileCount / 136; // new format
        if (version > 3)
            sf.originalVersion = this->version;
        else
            sf.originalVersion = 0;

        sf.version = version;

        std::vector<IndexInnerFile> index(sf.indexFileCount);

        for (const auto& file : this->index) {
            auto updated = file;
            updated.timestamp = 0; // LCE does this
            index.push_back(updated);
        }

        sf.index = index;
        return sf;
    }

    /**
     * Gets the size of an index entry based on the save file class type.
     * @return The size of an index entry
     */
    uint32_t SaveFileOld::getIndexEntrySize() {
        return 136;
    }

}