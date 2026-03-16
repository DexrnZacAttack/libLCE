//
// Created by DexrnZacAttack on 12/21/2024.
//

#include <BinaryIO/buffer/BinaryBuffer.h>

#include "LCE/save/SaveFile.h"

namespace lce::save {
    size_t SaveFile::getSize() const {
        uint64_t size = HEADER_SIZE +
            (getRoot()->getFileCount() *
             getIndexEntrySize()); // for each index entry there is 144 bytes
                                   // (136 bytes with old save file format)
        size += this->getRoot()->getSize();

        return size;
    }

    uint32_t SaveFile::calculateIndexOffset() const {
        return HEADER_SIZE + this->getRoot()->getSize();
    }

    SaveFile *
    SaveFile::deserializeAuto(std::vector<uint8_t> &data) {
        // const bio::util::ByteOrder e = detectByteOrder(data);
        //
        // if (const uint16_t v = getVersionFromData(data, e); v > B0033)
        //     return new SaveFile(data, e);
        //
        // return new SaveFileOld(data, e);

        return new SaveFile(1, 1);
    }

    uint16_t
    SaveFile::getVersionFromData(std::vector<uint8_t> &data,
                                        const bio::util::ByteOrder byteOrder) {
        bio::buffer::BinaryBuffer io(data.data());
        io.seek(10);
        return io.read<uint16_t>(byteOrder);
    }

    uint16_t SaveFile::getOriginalVersion() const {
        return this->m_originalVersion;
    }

    uint16_t SaveFile::getVersion() const { return this->m_version; }

    void SaveFile::setOriginalVersion(const uint16_t version) {
        this->m_originalVersion = version;
    }

    void SaveFile::setVersion(const uint16_t version) {
        this->m_version = version;
    }

    size_t SaveFile::getIndexEntrySize() const {
        if (this->m_version <= B0033) {
            return SaveFile::OLD_FILE_ENTRY_SIZE;
        }

        return SaveFile::FILE_ENTRY_SIZE;
    }
} // namespace lce::save
