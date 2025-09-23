//
// Created by DexrnZacAttack on 12/21/2024.
//

#include <IO/BinaryIO.h>
#include <Save/SaveFileCommons.h>

#include "Save/SaveFile.h"
#include "Save/SaveFileOld.h"

namespace lce::save {
    size_t SaveFileCommons::getSize() const {
        uint64_t size =
            HEADER_SIZE +
            (getRoot()->getFileCount() *
             getIndexEntrySize()); // for each index entry there is 144 bytes
                                   // (136 bytes with old save file format)
        size += this->getRoot()->getSize();

        return size;
    }

    uint32_t SaveFileCommons::calculateIndexOffset() const {
        return HEADER_SIZE + this->getRoot()->getSize();
    }

    SaveFileCommons *
    SaveFileCommons::deserializeAuto(std::vector<uint8_t> &data) {
        const io::ByteOrder e = detectByteOrder(data);

        if (const uint16_t v = getVersionFromData(data, e); v > B0033)
            return new SaveFile(data, e);

        return new SaveFileOld(data, e);
    }

    uint16_t
    SaveFileCommons::getVersionFromData(std::vector<uint8_t> &data,
                                        const io::ByteOrder byteOrder) {
        io::BinaryIO io(data.data());
        io.seek(10);
        return io.read<uint16_t>(byteOrder);
    }

    uint16_t SaveFileCommons::getOriginalVersion() const {
        return this->mOriginalVersion;
    }

    uint16_t SaveFileCommons::getVersion() const { return this->mVersion; }

    io::ByteOrder SaveFileCommons::getByteOrder() const {
        return this->mByteOrder;
    }

    void SaveFileCommons::setOriginalVersion(const uint16_t version) {
        this->mOriginalVersion = version;
    }

    void SaveFileCommons::setVersion(const uint16_t version) {
        this->mVersion = version;
    }

    void SaveFileCommons::setEndian(const io::ByteOrder byteOrder) {
        this->mByteOrder = byteOrder;
    }

    io::ByteOrder SaveFileCommons::detectByteOrder(std::vector<uint8_t> data) {
        io::BinaryIO io(data.data());

        io.seek(4 + 4 + 2);

        const uint8_t *s = io.getDataRelative();

        return *s != 0x00 ? io::ByteOrder::LITTLE : io::ByteOrder::BIG;
    }
} // namespace lce::save
