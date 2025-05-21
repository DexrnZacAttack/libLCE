//
// Created by DexrnZacAttack on 12/21/2024.
//

#include "SaveFileCommons.h"

#include <codecvt>
#include <cstdint>
#include <variant>
#include <algorithm>

#include "IndexInnerFile.h"
#include "SaveFileOld.h"
#include "SaveFile.h"
#include "../IO/BinaryIO.h"

#include <stdexcept> //Remove when implemented

namespace lce::save {
    class SaveFileOld;
    class SaveFile;

    uint64_t SaveFileCommons::getSize() const {
        uint64_t size = HEADER_SIZE + (getIndexCount() * getIndexEntrySize()); // for each index entry there is 144 bytes (136 bytes with old save file format)
        for (const auto& file: getIndex()) {
            size += file->getSize();
        }

        return size;
    }

    uint32_t SaveFileCommons::calculateIndexOffset() const {
        return HEADER_SIZE + this->getFilesSize();
    }

    uint32_t SaveFileCommons::getFilesSize() const {
        uint32_t size = 0;
        for (const auto& file: getIndex()) {
            size += file->getSize();
            DebugLog("Name: " + file->getName());
        }
        
        DebugLog("FileSize: " + std::to_string(size));
        DebugLog("FileCount: " + std::to_string(getIndexCount()));
		
        return size;
    }

#ifndef __EMSCRIPTEN__
    std::variant<SaveFile, SaveFileOld> SaveFileCommons::readAuto(std::vector<uint8_t> data, ByteOrder endian) {
        io::BinaryIO io(data.data());
        io.seek(10);

        if (const auto version = static_cast<SaveFileVersion>(io.read<uint16_t>(endian)); version > PR)
            return SaveFile(data, endian);

        return SaveFileOld(data, endian);
    }
#endif

    uint16_t SaveFileCommons::getVersionFromData(std::vector<uint8_t> data, ByteOrder endian) {
        io::BinaryIO io(data.data());
        io.seek(10);
        return io.read<uint16_t>(endian);
    }
    
    uint32_t SaveFileCommons::getIndexOffset() const {
        return calculateIndexOffset();
    }

    uint16_t SaveFileCommons::getOriginalVersion() const {
        return this->originalVersion;
    }

    uint16_t SaveFileCommons::getVersion() const {
        return this->version;
    }

    ByteOrder SaveFileCommons::getEndian() const {
        return this->endian;
    }
    
    uint8_t* SaveFileCommons::create() const {
		return nullptr; // Fix emscripten
	}

    void SaveFileCommons::setOriginalVersion(uint16_t version) {
        this->originalVersion = version;
    }

    void SaveFileCommons::setVersion(uint16_t version) {
        this->version = version;
    }

    void SaveFileCommons::setEndian(ByteOrder endian) {
        this->endian = endian;
    }
    
	/**
	* Gets the size of an index entry based on the save file class type.
	* @return The size of an index entry
	*/
	uint32_t SaveFileCommons::getIndexEntrySize() const {
		return 0;
	}
}
