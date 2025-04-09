//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "Archive.h"
#include "../IO/BinaryIO.h"

namespace lce::arc {
    Archive::Archive(uint32_t fileCount, const std::vector<ArchiveInnerFile> &index): fileCount(fileCount), index(index) {
    }

    Archive::Archive() = default;

    Archive::Archive(uint8_t *data) {
        io::BinaryIO io(data);

        this->fileCount = io.readBE<uint32_t>();

        std::vector<ArchiveInnerFile> index(this->fileCount);

        io::BinaryIO& io2 = io;

        for (uint32_t i = 0; i < this->fileCount; i++) {
            ArchiveInnerFile af(io2);
            uint32_t oldPos = io.getPosition();
            io.seek(af.offset);
            af.data = new uint8_t[af.size];
            io.readInto(af.data, af.size);
            index[i] = af;
            io.seek(oldPos);
        }

        this->index = index;
    }

    uint8_t *Archive::create() {
        const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        io::BinaryIO io(data);

        uint32_t *offsetPositions = new uint32_t[this->index.size()];
        uint32_t i = 0;

        io.writeBE<uint32_t>(this->index.size());
        for (auto file: this->index) {
            io.writeBE<uint16_t>(file.name.length());
            io.writeUtf8(file.name);
            // this stores the area where the file offset is written.
            offsetPositions[i] = io.getPosition();
            io.writeBE<uint32_t>(0);
            io.writeBE<uint32_t>(file.size);

            i++;
        }

        uint32_t j = 0;

        for (auto file: this->index) {
            // get current position (this is the position of the file)
            uint32_t pos = io.getPosition();
            // write the file
            io.writeBytes(file.data, file.size);
            // get the position after the file was written (we return here to write the next one)
            uint32_t pos2 = io.getPosition();
            // go to the offset offset (lol) and write the actual offset.
            io.seek(offsetPositions[j]);
            io.writeBE<uint32_t>(pos);
            // seek back over to the next file's position.
            io.seek(pos2);
            // and then we increment this.
            j++;
        }

        return io.getData();
    }

    uint32_t Archive::getSize() const {
        uint32_t size = 0;
        for (const auto& file: this->index) {
            size += 2; // string length prefix
            size += file.name.length(); // name length
            size += 8; // offset, size
            size += file.size; // file size
        }

        return size;
    }

    void Archive::addFile(const ArchiveInnerFile file) {
        this->index.push_back(file);
    }

    void Archive::removeFile(const uint32_t index) {
        const ArchiveInnerFile file = this->index[index];

        this->index.erase(this->index.begin() + index);
    }

}
