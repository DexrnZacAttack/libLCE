//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "Archive.h"
#include "../IO/BinaryIO.h"

namespace lce::arc {
    Archive::Archive(uint32_t fileCount, std::vector<fs::File>& index): fileCount(fileCount), Filesystem(index) {
    }

    Archive::Archive() = default;

    Archive::Archive(uint8_t *data) {
        io::BinaryIO io(data);

        this->fileCount = io.readBE<uint32_t>();

        std::vector<fs::File> index(this->fileCount);

        for (uint32_t i = 0; i < this->fileCount; i++) {
            uint16_t name_size = io.readBE<uint16_t>();
            std::string name = io.readUtf8(name_size);
			
			uint32_t offset = io.readBE<uint32_t>();
			uint32_t size = io.readBE<uint32_t>();
			uint8_t* data = new uint8_t[size];
            
            uint32_t oldPos = io.getPosition();
            io.seek(offset);
            io.readInto(data, size);
            io.seek(oldPos);
            
            fs::File af(name, size, offset, std::move(data));
            addFile(af);
        }
    }

    uint8_t* Archive::create() const {
        const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        io::BinaryIO io(data);

        uint32_t *offsetPositions = new uint32_t[getIndexSize()];
        uint32_t i = 0;

        io.writeBE<uint32_t>(getIndexSize());
        for (auto& file: getIndex()) {
            io.writeBE<uint16_t>(file.getName().length());
            io.writeUtf8(file.getName());
            // this stores the area where the file offset is written.
            offsetPositions[i] = io.getPosition();
            io.writeBE<uint32_t>(0);
            io.writeBE<uint32_t>(file.getSize());

            i++;
        }

        uint32_t j = 0;

        for (auto& file: getIndex()) {
            // get current position (this is the position of the file)
            uint32_t pos = io.getPosition();
            // write the file
            io.writeBytes(file.create(), file.getSize());
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

    uint64_t Archive::getSize() const {
        uint32_t size = 0;
        for (const auto& file: getIndex()) {
            size += 2; // string length prefix
            size += file.getName().length(); // name length
            size += 8; // offset, size
            size += file.getSize(); // file size
        }

        return size;
    }

}
