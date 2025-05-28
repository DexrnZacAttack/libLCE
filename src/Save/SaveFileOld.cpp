//
// Created by DexrnZacAttack on 12/21/2024.
//

#include "SaveFileOld.h"

#include <chrono>
#include "SaveFile.h"
#include "../IO/BinaryIO.h"
#include <stack>


namespace lce::save {
    SaveFileOld::SaveFileOld(ByteOrder endian) {
        this->endian = endian;
    }

    /**
     * Reads an old-format save file from a pointer to the data
     * @param data The data you want to read (a save file)
     * @param endian The endian/byte order of the save file (default Big)
     * @return The old-format save file.
     */
    SaveFileOld::SaveFileOld(std::vector<uint8_t> data, ByteOrder endian) {
        this->endian = endian;
        io::BinaryIO io(data.data());

        uint32_t indexOffset = io.read<uint32_t>(this->endian);

        if (indexOffset > data.size())
            throw std::runtime_error("Index offset points to an area that is out of bounds of the data given.");

        DebugLog(""<<io.getPosition());
        uint32_t fileCount = io.read<uint32_t>(this->endian) / this->SaveFileOld::getIndexEntrySize();

        if (fileCount > (0xFFFFFFFF - HEADER_SIZE) / this->SaveFileOld::getIndexEntrySize() )
            throw std::runtime_error("File count (" + std::to_string(fileCount) + ") makes the file too big for it's index offset to stored in a 32-bit integer.");

        this->originalVersion = io.read<uint16_t>(this->endian);
        this->version = io.read<uint16_t>(this->endian);

        DebugLog("Index offset: " << indexOffset);
        DebugLog("Index file count: " << fileCount);
        DebugLog("Version: " << this->version);
        DebugLog("Orig version: " << this->originalVersion);

        for (int i = 0; i < fileCount; ++i) {
            io.seek(indexOffset + (SaveFileOld::getIndexEntrySize() * i));
            // read the index entry

            std::wstring name = io.u16stringToWstring(io.readWChar2Byte(64, this->endian));
            io.trimWString(name);

            uint32_t size = io.read<uint32_t>(this->endian);
            uint32_t offset = io.read<uint32_t>(this->endian);

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
    uint8_t* SaveFileOld::toData() const {
          io::BinaryIO io(this->getSize());
        fs::Directory *root = getRoot();

        uint32_t indexOffset = calculateIndexOffset();

        if (indexOffset > 0xFFFFFFFF - HEADER_SIZE)
            throw std::runtime_error("Index offset is too big to be stored in a 32-bit integer.");

        io.write<uint32_t>(0, this->endian);
        io.write<uint32_t>(root->getFileCount() * getIndexEntrySize(), this->endian);
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

                io.seek(last);
                i++;
            }
        }

        io.seek(0);
        io.write<uint32_t>(indexOffset, this->endian);

        return io.getData();
    }

    /**
     * Migrates from SaveFileOld to SaveFile
     * @param version The version you want to migrate to (has no effect except for changing the version in the header)
     * @return A shared_ptr to the SaveFile
     */
    SaveFile *SaveFileOld::upgrade(uint16_t version = 2) {
        if (version <= 1)
            throw std::invalid_argument("Version must be greater than 1. (otherwise it's not really a migration now is it?)");

        const uint32_t indexFileCount = getRoot()->getFileCount(); // new format

        uint16_t originalVersion;
        if (version > 3)
            originalVersion = this->version;
        else
            originalVersion = 0;

        SaveFile *s = new SaveFile(indexFileCount, originalVersion, version);

        fs::Directory *root = getRoot();
        for (const auto& [name, child] : root->getChildren())
        {
            root->moveChild(name, s->getRoot());
        }

        return s;
    }

}
