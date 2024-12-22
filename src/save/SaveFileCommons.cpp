//
// Created by DexrnZacAttack on 12/21/2024.
//

#include "SaveFileCommons.h"

#include <cstdint>
#include <variant>

#include "IndexInnerFile.h"
#include "SaveFileOld.h"
#include "SaveFile.h"
#include "../io/BinaryIO.h"

namespace lce::save {
    class SaveFileOld;
    class SaveFile;

    void SaveFileCommons::addFile(const IndexInnerFile file) {
        this->index.push_back(file);
        this->indexFileCount++;
        this->indexOffset += file.size;
    }

    void SaveFileCommons::removeFile(const uint32_t index) {
        const IndexInnerFile file = this->index[index];

        this->indexOffset -= file.size;
        this->index.erase(this->index.begin() + index);
        this->indexFileCount--;
    }

    uint32_t SaveFileCommons::getSize() {
        uint32_t size = HEADER_SIZE + (this->indexFileCount * getIndexEntrySize()); // for each index entry there is 144 bytes
        for (const auto& file: this->index) {
            size += file.size;
        }

        return size;
    }

    uint32_t SaveFileCommons::getFilesSize() const {
        uint32_t size = 0;
        for (const auto& file: this->index) {
            size += file.size;
        }

        return size;
    }


    /**
     * Gets the size of an index entry based on the save file class type.
     * @return The size of an index entry
     */
    uint32_t SaveFileCommons::getIndexEntrySize() {
        return 0;
    }

    std::variant<SaveFile, SaveFileOld> SaveFileCommons::readFromDataAuto(uint8_t *data) {
        io::BinaryIO io(data);
        io.seek(10);

        if (const auto version = static_cast<SaveFileVersion>(io.readB<uint16_t>()); version > PR)
            return SaveFile::readFromData(data);

        return SaveFileOld::readFromData(data);
    }
}
