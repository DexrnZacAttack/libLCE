//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "save/SaveFileOld.h"

#include <chrono>
#include <stack>
#include <vector>

#include "save/SaveFile.h"

#include "BinaryIO/util/string/StringConverter.h"
#include "util/StringUtilities.h"

#include <BinaryIO/buffer/BinaryBuffer.h>

namespace lce::save {
    SaveFile::SaveFile(const bio::util::ByteOrder byteOrder,
                       const uint16_t origVersion, const uint16_t version)
        : SaveFileCommons(byteOrder, origVersion, version) {}

    SaveFile::SaveFile(const Filesystem &fs,
                       const bio::util::ByteOrder byteOrder,
                       const uint16_t origVersion, const uint16_t version)
        : SaveFileCommons(fs, byteOrder, origVersion, version) {};

    /**
     * Reads a save file from a pointer to the data
     * @param data The data you want to read (a save file)
     * @param byteOrder The endianness of the data being read
     */
    SaveFile::SaveFile(std::vector<uint8_t> data,
                       bio::util::ByteOrder byteOrder) {
        this->mByteOrder = byteOrder;
        bio::buffer::BinaryBuffer io(data.data());

        const uint32_t indexOffset = io.read<uint32_t>(this->mByteOrder);

        if (indexOffset > data.size())
            throw std::runtime_error("Index offset points to an area that is "
                                     "out of bounds of the data given.");

        DebugLog("" << io.getOffset());
        const uint32_t fileCount = io.read<uint32_t>(this->mByteOrder);

        if (fileCount >
            (0xFFFFFFFF - HEADER_SIZE) / this->SaveFile::getIndexEntrySize())
            throw std::runtime_error("File count (" +
                                     std::to_string(fileCount) +
                                     ") makes the file too big for it's index "
                                     "offset to stored in a 32-bit integer.");

        this->mOriginalVersion = io.read<uint16_t>(this->mByteOrder);
        this->mVersion = io.read<uint16_t>(this->mByteOrder);

        if (this->mVersion <= B0033) {
            throw std::runtime_error("Version mismatch, got version " +
                                     std::to_string(this->mVersion) + ".");
        }

        DebugLog("Index offset: " << indexOffset);
        DebugLog("Index file count: " << fileCount);
        DebugLog("Version: " << this->mVersion);
        DebugLog("Orig version: " << this->mOriginalVersion);

        for (int i = 0; i < fileCount; ++i) {
            io.seek(indexOffset + (SaveFile::getIndexEntrySize() * i));
            // read the index entry

            std::wstring name =
                bio::util::string::StringConverter::u16stringToWstring(
                    io.readU16String(64, this->mByteOrder));
            lce::util::StringUtilities::trimEndNullBytes(name);

            const uint32_t size = io.read<uint32_t>(this->mByteOrder);
            const uint32_t offset = io.read<uint32_t>(this->mByteOrder);
            const uint64_t modifiedTimestamp =
                io.read<uint64_t>(this->mByteOrder); // unused for now

            // read the data, maybe should be changed
            io.seek(offset);

            std::vector<uint8_t> d;
            d.resize(size);
            io.readInto(d.data(), size);

            // create the file
            (void)this->createFileRecursive(name, d);
        }
    }

    /**
     * Writes the save file
     * @return Pointer to the save file
     */
    uint8_t *SaveFile::serialize() const {
        bio::buffer::BinaryBuffer io(this->getSize());
        const fs::Directory *root = getRoot();

        uint32_t indexOffset = calculateIndexOffset();

        if (indexOffset > 0xFFFFFFFF - HEADER_SIZE)
            throw std::runtime_error(
                "Index offset is too big to be stored in a 32-bit integer.");

        io.write<uint32_t>(0, this->mByteOrder);
        io.write<uint32_t>(root->getFileCount(), this->mByteOrder);
        io.write<uint16_t>(this->mOriginalVersion, this->mByteOrder);
        io.write<uint16_t>(this->mVersion, this->mByteOrder);

        size_t i = 0;
        root->forEachFilesRecursive([&i, &io, &indexOffset,
                                     this](const std::wstring &name,
                                           const fs::File &innerFile) {
            const std::wstring path = innerFile.getPath().substr(1);

            const uint32_t offset = io.getOffset();

            io.writeBytes(innerFile.begin().data(), innerFile.getSize());

            const size_t last = io.getOffset();

            io.seek(indexOffset + (getIndexEntrySize() * i));

            std::u16string n =
                bio::util::string::StringConverter::wstringToU16string(path);

            if (path.length() > 64) {
                std::wcerr << L"Filename '" << path
                           << L"' is too long. The path will be truncated, "
                              L"however this may cause weird paths, invalid "
                              L"names, or other issues in the output file."
                           << std::endl;
            }
            n.resize(64);

            io.writeU16String(n, this->mByteOrder, false);
            io.write<uint32_t>(innerFile.getSize(), this->mByteOrder);
            io.write<uint32_t>(offset, this->mByteOrder);
            io.write<uint64_t>(innerFile.getModifiedTimestamp(),
                               this->mByteOrder);

            io.seek(last);
            i++;
        });

        io.seek(0);
        io.write<uint32_t>(indexOffset, this->mByteOrder);

        return io.begin();
    }

    SaveFileCommons *SaveFile::migrateVersion(const uint16_t version) {
        uint16_t originalVersion;
        if (version > TU5)
            originalVersion = this->mVersion;
        else
            originalVersion = 0;

        if (version != B0033) {
            this->setVersion(version);
            this->setOriginalVersion(originalVersion);

            return this;
        }

        return new SaveFileOld(*this, this->mByteOrder, originalVersion,
                               version);
    }

} // namespace lce::save
