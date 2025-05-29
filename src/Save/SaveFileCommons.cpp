//
// Created by DexrnZacAttack on 12/21/2024.
//

#include <IO/BinaryIO.h>
#include <Save/SaveFileCommons.h>

#include <algorithm>
#include <codecvt>
#include <cstdint>
#include <variant>

#include <stdexcept> //Remove when implemented

namespace lce::save {
    class SaveFileOld;
    class SaveFile;

    uint64_t SaveFileCommons::getSize() const {
        uint64_t size =
            HEADER_SIZE +
            (getRoot()->getFileCount() *
             getIndexEntrySize()); // for each index entry there is 144 bytes (136 bytes with old save file format)
        size += this->getRoot()->getSize();

        return size;
    }

    uint32_t SaveFileCommons::calculateIndexOffset() const { return HEADER_SIZE + this->getRoot()->getSize(); }

    uint16_t SaveFileCommons::getVersionFromData(std::vector<uint8_t> data, io::ByteOrder endian) {
        io::BinaryIO io(data.data());
        io.seek(10);
        return io.read<uint16_t>(endian);
    }

    uint16_t SaveFileCommons::getOriginalVersion() const { return this->originalVersion; }

    uint16_t SaveFileCommons::getVersion() const { return this->version; }

    io::ByteOrder SaveFileCommons::getEndian() const { return this->endian; }

    void SaveFileCommons::setOriginalVersion(uint16_t version) { this->originalVersion = version; }

    void SaveFileCommons::setVersion(uint16_t version) { this->version = version; }

    void SaveFileCommons::setEndian(io::ByteOrder endian) { this->endian = endian; }
} // namespace lce::save
