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
    SaveFile::SaveFile(uint32_t indexOffset, uint32_t indexFileCount, uint16_t origVersion, uint16_t version,
        const std::vector<IndexInnerFile> &index) {
    }

    SaveFile::SaveFile() = default;

    SaveFile::SaveFile(ByteOrder endian) {
        this->endian = endian;
    }

    /**
     * Reads a save file from a pointer to the data
     * @param data The data you want to read (a save file)
     * @return The save file.
     */
    SaveFile::SaveFile(std::vector<uint8_t> data, ByteOrder endian) {
        this->endian = endian;
        io::BinaryIO io((data.data()));

        this->indexOffset = io.read<uint32_t>(this->endian);

        if (this->indexOffset > data.size())
            throw std::runtime_error("Index offset points to an area that is out of bounds of the data given.");

        this->indexFileCount = io.read<uint32_t>(this->endian);
        this->originalVersion = io.read<uint16_t>(this->endian);
        this->version = io.read<uint16_t>(this->endian);

        std::vector<IndexInnerFile> index(this->indexFileCount);

        for (int i = 0; i < this->indexFileCount; ++i) {
            io.seek(this->indexOffset + (144 * i));
            // read the index entry
            IndexInnerFile inf = IndexInnerFile(io.readOfSize(144), false, this->endian);
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
    const uint8_t *SaveFile::create() {
        io::BinaryIO io(this->getSize());

        this->indexOffset = HEADER_SIZE;

        io.write<uint32_t>(0, this->endian);
        io.write<uint32_t>(this->indexFileCount, this->endian);
        io.write<uint16_t>(this->originalVersion, this->endian);
        io.write<uint16_t>(this->version, this->endian);

        for (const auto& file: this->index) {
            file.offset = io.getPosition();
            file.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            io.writeBytes(file.data, file.size);
            indexOffset += file.size;
        }

        for (const auto& file: this->index) {
            std::wcout << file.name.length() << std::endl;
            io.writeWChar2Byte(file.name, this->endian);
            io.write<uint32_t>(file.size, this->endian);
            io.write<uint32_t>(file.offset, this->endian);
            io.write<uint64_t>(file.timestamp, this->endian);
        }

        io.seek(0);
        io.write<uint32_t>(this->indexOffset, this->endian);

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