//
// Created by DexrnZacAttack on 12/21/2024.
//

#include "SaveFileCommons.h"

#include <codecvt>
#include <cstdint>
#include <variant>

#include "IndexInnerFile.h"
#include "SaveFileOld.h"
#include "SaveFile.h"
#include "../io/BinaryIO.h"

namespace lce::save {
    class SaveFileOld;
    class SaveFile;

    void SaveFileCommons::addFile(const IndexInnerFile &file) {
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

    std::optional<IndexInnerFile> SaveFileCommons::getFileByName(std::wstring name) {
        const auto find = std::find_if(index.begin(), index.end(), [&name](const IndexInnerFile& file) {
            return file.name == name;
        });

        if (find != index.end()) return *find;

        return std::nullopt;
    }

    uint32_t SaveFileCommons::calculateIndexOffset() const {
        return HEADER_SIZE + this->getFilesSize();
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

    std::variant<SaveFile, SaveFileOld> SaveFileCommons::readAuto(std::vector<uint8_t> data) {
        io::BinaryIO io(data.data());
        io.seek(10);

        if (const auto version = static_cast<SaveFileVersion>(io.readBE<uint16_t>()); version > PR)
            return SaveFile::SaveFile(data);

        return SaveFileOld::SaveFileOld(data);
    }

    uint32_t SaveFileCommons::getFileCount() const {
        return this->indexFileCount;
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

    void SaveFileCommons::setOriginalVersion(uint16_t version) {
        this->originalVersion = version;
    }

    void SaveFileCommons::setVersion(uint16_t version) {
        this->version = version;
    }

    void SaveFileCommons::setEndian(ByteOrder endian) {
        this->endian = endian;
    }
}
