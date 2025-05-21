//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "SaveFile.h"

#include <any>
#include <chrono>
#include <fstream>
#include <vector>

#include "IndexInnerFile.h"
#include "../IO/BinaryIO.h"

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
        io::BinaryIO io(data.data());

        this->indexOffset = io.read<uint32_t>(this->endian);

        if (this->indexOffset > data.size())
            throw std::runtime_error("Index offset points to an area that is out of bounds of the data given.");

        this->indexFileCount = io.read<uint32_t>(this->endian);

        if (this->indexFileCount > (0xFFFFFFFF - HEADER_SIZE) / this->SaveFile::getIndexEntrySize())
            throw std::runtime_error("File count (" + std::to_string(this->indexFileCount) + ") makes the file too big for it's index offset to stored in a 32-bit integer.");

        this->originalVersion = io.read<uint16_t>(this->endian);
        this->version = io.read<uint16_t>(this->endian);

        DebugLog("Index offset: " << this->indexOffset);
        DebugLog("Index file count: " << this->indexFileCount);
        DebugLog("Version: " << this->version);
        DebugLog("Orig version: " << this->originalVersion);

        std::vector<IndexInnerFile> index(this->indexFileCount);

        for (int i = 0; i < this->indexFileCount; ++i) {
            io.seek(this->indexOffset + (144 * i));
            // read the index entry
            IndexInnerFile inf = IndexInnerFile(io.readOfSize(144), false, this->endian);
            
            DebugLogW(io::BinaryIO::u16stringToWstring(inf.getName()));

            // read the data, maybe should be changed
            io.seek(inf.getOffset());
            inf.setData(new uint8_t[inf.getSize()]);
            io.readInto(inf.getData(), inf.getSize());
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

        this->indexOffset = this->calculateIndexOffset();

        if (this->getIndexOffset() > 0xFFFFFFFF - HEADER_SIZE)
            throw std::runtime_error("Index offset is too big to be stored in a 32-bit integer.");

        io.write<uint32_t>(0, this->endian);
        io.write<uint32_t>(this->indexFileCount, this->endian);
        io.write<uint16_t>(this->originalVersion, this->endian);
        io.write<uint16_t>(this->version, this->endian);

        for (auto& file: this->index) {
            file.setOffset(io.getPosition());
            file.setTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
            io.writeBytes(file.getData(), file.getSize());
        }

        io.seek(this->indexOffset);
        for (const auto& file: this->index) {
            DebugLog(file.getName().length());
            io.writeWChar2Byte(file.getNameU16(), this->endian, false);
            io.write<uint32_t>(file.getSize(), this->endian);
            io.write<uint32_t>(file.getOffset(), this->endian);
            io.write<uint64_t>(file.getTimestamp(), this->endian);
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
