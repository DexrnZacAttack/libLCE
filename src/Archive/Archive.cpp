//
// Created by DexrnZacAttack on 12/19/2024.
//

#include <Archive/Archive.h>
#include <IO/BinaryIO.h>

namespace lce::arc {
    Archive::Archive(const Filesystem &fs) : Filesystem(fs) {};
    Archive::Archive() = default;

    Archive::Archive(std::vector<uint8_t> data) : Archive(data.data()) {}

    Archive::Archive(uint8_t *data) {
        io::BinaryIO io(data);
        const uint32_t fileCount = io.readBE<uint32_t>();

        for (uint32_t i = 0; i < fileCount; i++) {
            std::string name = io.readUtf8(io.readBE<uint16_t>());

            const uint32_t offset = io.readBE<uint32_t>();
            const uint32_t size = io.readBE<uint32_t>();

            std::vector<uint8_t> d;
            d.resize(size);

            const uint32_t oldPos = io.getPosition();
            io.seek(offset);
            io.readInto(d.data(), size);
            io.seek(oldPos);

            std::wstring wname = io::BinaryIO::stringToWString(name);
            windowsToUnixDelimiter(wname); // convert paths

            this->createFileRecursive(wname, d);
        }
    }

    uint8_t *Archive::serialize() const {
        io::BinaryIO io(new uint8_t[this->getSize()]);

        const fs::Directory *root = getRoot();

        const size_t count = root->getFileCount();
        io.writeBE<uint32_t>(count);

        uint32_t *offsetPositions = new uint32_t[count];
        uint32_t i = 0;
        root->forEachFilesRecursive(
            [&i, &io, &offsetPositions](const std::wstring &n,
                                        const fs::File &f) {
                std::wstring path = f.getPath().substr(1);
                unixToWindowsDelimiter(path);

                io.writeBE<uint16_t>(path.length());
                io.writeUtf8(io::BinaryIO::wstringToString(path));
                // this stores the area where the file offset is written.
                offsetPositions[i] = io.getPosition();
                io.writeBE<uint32_t>(0);
                io.writeBE<uint32_t>(f.getSize());

                i++;
            });

        uint32_t j = 0;
        root->forEachFilesRecursive(
            [&j, &io, &offsetPositions](const std::wstring &n,
                                        const fs::File &f) {
                // get current position (this is the position of the file)
                const uint32_t fPos = io.getPosition();
                // write the file
                io.writeBytes(f.getData().data(), f.getSize());

                // get the position after the file was written (we return here
                // to write the next one)
                const uint32_t last = io.getPosition();

                // go to the offset's offset (lol) and write the actual offset.
                io.seek(offsetPositions[j]);
                io.writeBE<uint32_t>(fPos);

                // seek back over to the next file's position.
                io.seek(last);

                // and then we increment this.
                j++;
            });

        return io.getData();
    }

    size_t Archive::getSize() const {
        uint32_t size = 4; // file count

        getRoot()->forEachFilesRecursive(
            [&size](std::wstring n, const fs::File &f) {
                std::wstring path = f.getPath().substr(1);
                unixToWindowsDelimiter(path);

                size += 2;             // string length prefix
                size += path.length(); // name length
                size += 8;             // offset, size
                size += f.getSize();   // file size
            });

        return size;
    }

} // namespace lce::arc
