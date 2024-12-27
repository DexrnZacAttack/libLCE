//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "SaveFile.h"

#include <any>
#include <chrono>
#include <fstream>
#include <vector>

#include "IndexInnerFile.h"
#include "../io/BinaryIO.h"

namespace lce::save {
    // TODO: little endian support

    SaveFile::SaveFile(uint32_t indexOffset, uint32_t indexFileCount, uint16_t origVersion, uint16_t version,
        const std::vector<IndexInnerFile> &index) {
    }

    SaveFile::SaveFile() = default;

    /**
     * Reads a save file from a pointer to the data
     * @param data The data you want to read (a save file)
     * @return The save file.
     */
    SaveFile SaveFile::read(std::vector<uint8_t> data) {
        SaveFile sf;
        io::BinaryIO io((data.data()));

        sf.indexOffset = io.readB<uint32_t>();
        sf.indexFileCount = io.readB<uint32_t>();
        sf.originalVersion = io.readB<uint16_t>();
        sf.version = io.readB<uint16_t>();

        std::vector<IndexInnerFile> index(sf.indexFileCount);

        for (int i = 0; i < sf.indexFileCount; ++i) {
            io.seek(sf.indexOffset + (144 * i));
            // read the index entry
            IndexInnerFile inf = IndexInnerFile::read(io.readOfSize(144));
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
    const uint8_t *SaveFile::create() {
        const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        io::BinaryIO io(data);

        this->indexOffset = HEADER_SIZE;

        io.writeB<uint32_t>(0);
        io.writeB<uint32_t>(this->indexFileCount);
        io.writeB<uint16_t>(this->originalVersion);
        io.writeB<uint16_t>(this->version);

        for (const auto& file: this->index) {
            file.offset = io.getPosition();
            file.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            io.writeBytes(file.data, file.size);
            indexOffset += file.size;
        }

        for (const auto& file: this->index) {
            io.writeWChar2ByteB(file.name);
            io.writeB<uint32_t>(file.size);
            io.writeB<uint32_t>(file.offset);
            io.writeB<uint64_t>(file.timestamp);
        }

        io.seek(0);
        io.writeB<uint32_t>(this->indexOffset);

        return io.getData();
    }

    /**
     * Gets the size of an index entry based on the save file class type.
     * @return The size of an index entry
     */
    uint32_t SaveFile::getIndexEntrySize() {
        return 144;
    }
} // lce