//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "SaveFile.h"

#include <chrono>
#include <stack>
#include <vector>

#include "../IO/BinaryIO.h"

namespace lce::save {
    SaveFile::SaveFile(uint32_t indexFileCount, uint16_t origVersion, uint16_t version) {
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

        uint32_t indexOffset = io.read<uint32_t>(this->endian);

        if (indexOffset > data.size())
            throw std::runtime_error("Index offset points to an area that is out of bounds of the data given.");
		
		DebugLog(""<<io.getPosition());
        uint32_t fileCount = io.read<uint32_t>(this->endian);

        if (fileCount > (0xFFFFFFFF - HEADER_SIZE) / this->SaveFile::getIndexEntrySize() )
            throw std::runtime_error("File count (" + std::to_string(fileCount) + ") makes the file too big for it's index offset to stored in a 32-bit integer.");

        this->originalVersion = io.read<uint16_t>(this->endian);
        this->version = io.read<uint16_t>(this->endian);

        DebugLog("Index offset: " << indexOffset);
        DebugLog("Index file count: " << fileCount);
        DebugLog("Version: " << this->version);
        DebugLog("Orig version: " << this->originalVersion);

        for (int i = 0; i < fileCount; ++i) {
            io.seek(indexOffset + (SaveFile::getIndexEntrySize() * i));
            // read the index entry

            std::wstring name = io.u16stringToWstring(io.readWChar2Byte(64, this->endian));
            io.trimWString(name);

            uint32_t size = io.read<uint32_t>(this->endian);
            uint32_t offset = io.read<uint32_t>(this->endian);
            uint64_t modifiedTimestamp = io.read<uint64_t>(this->endian); // unused for now
            
            DebugLogW(name);

            // read the data, maybe should be changed
            io.seek(offset);

            std::vector<uint8_t> d;
            d.resize(size);
            io.readInto(d.data(), size);

            // create the file
            this->createFileRecursive(name, d);
        }
    }

    /**
     * Writes the save file
     * @return Pointer to the save file
     */
    uint8_t* SaveFile::toData() const {
        io::BinaryIO io(this->getSize());
        fs::Directory *root = getRoot();

        uint32_t indexOffset = calculateIndexOffset();

        if (indexOffset > 0xFFFFFFFF - HEADER_SIZE)
            throw std::runtime_error("Index offset is too big to be stored in a 32-bit integer.");

        io.write<uint32_t>(0, this->endian);
        io.write<uint32_t>(root->getFileCount(), this->endian);
        io.write<uint16_t>(this->originalVersion, this->endian);
        io.write<uint16_t>(this->version, this->endian);

        size_t i = 0;

        // wtf I've never heard of this till now
        std::stack<const fs::Directory*> stack;
        stack.push(root);

        while (!stack.empty()) {
            const fs::Directory* d = stack.top();
            stack.pop();

            for (const auto& [name, child] : d->getChildren()) {
                if (!child->isFile()) {
                    stack.push(dynamic_cast<const fs::Directory*>(child.get()));
                    continue;
                }

                const fs::File* innerFile = dynamic_cast<const fs::File*>(child.get());
                if (!innerFile) continue;

                std::wstring path = innerFile->getPath().substr(1);

                DebugLogW(path);

                uint32_t offset = io.getPosition();

                io.writeBytes(innerFile->getData().data(), innerFile->getSize());

                size_t last = io.getPosition();

                io.seek(indexOffset + (getIndexEntrySize() * i));

                std::u16string n = io.wstringToU16string(path);
                n.resize(64);

                io.writeWChar2Byte(n, this->endian, false);
                io.write<uint32_t>(innerFile->getSize(), this->endian);
                io.write<uint32_t>(offset, this->endian);
                io.write<uint64_t>(innerFile->getModifiedTimestamp(), this->endian);

                io.seek(last);
                i++;
            }
        }

        io.seek(0);
        io.write<uint32_t>(indexOffset, this->endian);

        return io.getData();
    }

} // lce
