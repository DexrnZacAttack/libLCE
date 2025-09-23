//
// Created by DexrnZacAttack on 12/21/2024.
//

#include <IO/BinaryIO.h>
#include <Save/SaveFile.h>
#include <Save/SaveFileOld.h>

#include <chrono>
#include <stack>

namespace lce::save {
    SaveFileOld::SaveFileOld(const io::ByteOrder byteOrder,
                             const uint16_t origVersion, const uint16_t version)
        : SaveFileCommons(byteOrder, origVersion, version) {}

    SaveFileOld::SaveFileOld(const Filesystem &fs,
                             const io::ByteOrder byteOrder,
                             const uint16_t origVersion, const uint16_t version)
        : SaveFileCommons(fs, byteOrder, origVersion, version) {};

    /**
     * Reads an old-format save file from a pointer to the data
     * @param data The data you want to read (a save file)
     * @param byteOrder The byte order/endianness of the save file (default Big)
     */
    SaveFileOld::SaveFileOld(std::vector<uint8_t> data,
                             const io::ByteOrder byteOrder) {
        this->mByteOrder = byteOrder;
        io::BinaryIO io(data.data());

        const uint32_t indexOffset = io.read<uint32_t>(this->mByteOrder);

        if (indexOffset > data.size())
            throw std::runtime_error("Index offset points to an area that is "
                                     "out of bounds of the data given.");

        DebugLog("" << io.getPosition());
        const uint32_t fileCount = io.read<uint32_t>(this->mByteOrder) /
                                   this->SaveFileOld::getIndexEntrySize();

        if (fileCount >
            (0xFFFFFFFF - HEADER_SIZE) / this->SaveFileOld::getIndexEntrySize())
            throw std::runtime_error("File count (" +
                                     std::to_string(fileCount) +
                                     ") makes the file too big for it's index "
                                     "offset to stored in a 32-bit integer.");

        this->mOriginalVersion = io.read<uint16_t>(this->mByteOrder);
        this->mVersion = io.read<uint16_t>(this->mByteOrder);

        if (this->mVersion > B0033) {
            throw std::runtime_error("Version mismatch, got version " +
                                     std::to_string(this->mVersion) + ".");
        }

        DebugLog("Index offset: " << indexOffset);
        DebugLog("Index file count: " << fileCount);
        DebugLog("Version: " << this->mVersion);
        DebugLog("Orig version: " << this->mOriginalVersion);

        for (int i = 0; i < fileCount; ++i) {
            io.seek(indexOffset + (SaveFileOld::getIndexEntrySize() * i));
            // read the index entry

            std::wstring name = lce::io::BinaryIO::u16stringToWstring(
                io.readWChar2Byte(64, this->mByteOrder));
            lce::io::BinaryIO::trimWString(name);

            const uint32_t size = io.read<uint32_t>(this->mByteOrder);
            const uint32_t offset = io.read<uint32_t>(this->mByteOrder);

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
    uint8_t *SaveFileOld::serialize() const {
        io::BinaryIO io(this->getSize());
        const fs::Directory *root = getRoot();

        uint32_t indexOffset = calculateIndexOffset();

        if (indexOffset > 0xFFFFFFFF - HEADER_SIZE)
            throw std::runtime_error(
                "Index offset is too big to be stored in a 32-bit integer.");

        io.write<uint32_t>(0, this->mByteOrder);
        io.write<uint32_t>(root->getFileCount() * getIndexEntrySize(),
                           this->mByteOrder);
        io.write<uint16_t>(this->mOriginalVersion, this->mByteOrder);
        io.write<uint16_t>(this->mVersion, this->mByteOrder);

        size_t i = 0;

        root->forEachFilesRecursive([&i, &io, &indexOffset,
                                     this](const std::wstring &name,
                                           const fs::File &innerFile) {
            const std::wstring path = innerFile.getPath().substr(1);

            DebugLogW(path);

            const uint32_t offset = io.getPosition();

            io.writeBytes(innerFile.getData().data(), innerFile.getSize());

            const size_t last = io.getPosition();

            io.seek(indexOffset + (getIndexEntrySize() * i));

            std::u16string n = io::BinaryIO::wstringToU16string(path);

            if (path.length() > 64) {
                std::wcerr << L"Filename '" << path
                           << L"' is too long. The path will be truncated, "
                              L"however this may cause weird paths, invalid "
                              L"paths, or other issues in the output file."
                           << std::endl;
            }
            n.resize(64);

            io.writeWChar2Byte(n, this->mByteOrder, false);
            io.write<uint32_t>(innerFile.getSize(), this->mByteOrder);
            io.write<uint32_t>(offset, this->mByteOrder);

            io.seek(last);
            i++;
        });

        io.seek(0);
        io.write<uint32_t>(indexOffset, this->mByteOrder);

        return io.getData();
    }

    SaveFileCommons *SaveFileOld::migrateVersion(const uint16_t version) {
        uint16_t originalVersion;
        if (version > 3)
            originalVersion = this->mVersion;
        else
            originalVersion = 0;

        if (version <= B0033) {
            this->setVersion(version);
            this->setOriginalVersion(originalVersion);
            return this;
        }

        return new SaveFile(*this, this->mByteOrder, originalVersion, version);
    }

} // namespace lce::save
