//
// Created by DexrnZacAttack on 12/21/2024.
//

#include "SaveFileOld.h"

#include <chrono>
#include "IndexInnerFile.h"
#include "SaveFile.h"
#include "../IO/BinaryIO.h"

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
        const uint32_t indexSize = this->SaveFileOld::getIndexEntrySize();

        this->indexOffset = io.read<uint32_t>(this->endian);

        if (this->indexOffset > data.size())
            throw std::runtime_error("Index offset points to an area out of bounds of the data given.");

        if (getIndexSize() > (0xFFFFFFFF - HEADER_SIZE) / indexSize)
            throw std::runtime_error("File count (" + std::to_string(getIndexSize()) + ") makes the file too big for it's index offset to stored in a 32-bit integer.");

        resizeTo(io.read<uint32_t>(this->endian) / indexSize);
        this->originalVersion = io.read<uint16_t>(this->endian);
        this->version = io.read<uint16_t>(this->endian);

        for (int i = 0; i < getIndexSize(); ++i) {
            io.seek(this->indexOffset + (indexSize * i));
            // read the index entry
            IndexInnerFile inf(io.readOfSize(indexSize), true, this->endian);
            // read the data, maybe should be changed
            io.seek(inf.getOffset());
            inf.setData(new uint8_t[inf.getSize()]);
            io.readInto(inf.getData(), inf.getSize());
            // set the entry
            addFile(std::make_shared<IndexInnerFile>(inf));
        }
        
        for (auto& file: getIndex()) {
            file->setOffset(io.getPosition());
            io.writeBytes(file->getData(), file->getSize());
        }
    }

    /**
     * Writes the save file
     * @return Pointer to the save file
     */
    uint8_t* SaveFileOld::create() const {
        io::BinaryIO io(this->getSize());

        uint32_t indexOffset = this->calculateIndexOffset();

        if (this->getIndexOffset() > 0xFFFFFFFF - HEADER_SIZE)
            throw std::runtime_error("Index offset is too big to be stored in a 32-bit integer.");

        io.write<uint32_t>(0, this->endian);
        io.write<uint32_t>(getIndexSize() * this->getIndexEntrySize(), this->endian);
        io.write<uint16_t>(this->originalVersion, this->endian);
        io.write<uint16_t>(this->version, this->endian);

        io.seek(this->indexOffset);
        for (auto& file: getIndex()) {
            io.writeWChar2Byte(file->getNameU16(), this->endian, false);
            io.write<uint32_t>(file->getSize(), this->endian);
            io.write<uint32_t>(file->getOffset(), this->endian);
        }

        io.seek(0);
        io.write<uint32_t>(this->indexOffset, this->endian);

        return io.getData();
    }

    /**
     * Migrates from SaveFileOld to SaveFile
     * @param version The version you want to migrate to (has no effect except for changing the version in the header)
     * @return A shared_ptr to the SaveFile
     */
    SaveFile *SaveFileOld::migrate(uint16_t version = 2) {
        if (version <= 1)
            throw std::invalid_argument("Version must be greater than 1. (otherwise it's not really a migration now is it?)");

        const uint32_t indexOffset = this->indexOffset;
        const uint32_t indexFileCount = getIndexSize() / 136; // new format

        uint16_t originalVersion;
        if (version > 3)
            originalVersion = this->version;
        else
            originalVersion = 0;
            
        std::vector<std::shared_ptr<IndexInnerFile>> innerFiles;

		for (auto& file : getIndex()) {
			if (auto innerFile = std::dynamic_pointer_cast<IndexInnerFile>(file)) {
				innerFile->setTimestamp(0);
				innerFiles.push_back(innerFile);
			}
		}
		
        return new SaveFile(indexOffset, indexFileCount, originalVersion, version, innerFiles);
    }

    /**
     * Gets the size of an index entry based on the save file class type.
     * @return The size of an index entry
     */
    uint32_t SaveFileOld::getIndexEntrySize() const {
        return 136;
    }

}
