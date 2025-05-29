//
// Created by DexrnZacAttack on 12/19/2024.
//

#include <Archive/Archive.h>
#include <IO/BinaryIO.h>
#include <iostream>
#include <stack>

namespace lce::arc {
    // Archive::Archive(uint32_t fileCount, std::vector<std::shared_ptr<fs::File>>& index): fileCount(fileCount),
    // Filesystem(index) {
    // }

    Archive::Archive() = default;

    Archive::Archive(uint8_t* data) {
        io::BinaryIO io(data);
        this->fileCount = io.readBE<uint32_t>();

        for (uint32_t i = 0; i < this->fileCount; i++) {
            std::string name = io.readUtf8(io.readBE<uint16_t>());

            uint32_t offset = io.readBE<uint32_t>();
            uint32_t size = io.readBE<uint32_t>();

            std::vector<uint8_t> d;
            d.resize(size);

            uint32_t oldPos = io.getPosition();
            io.seek(offset);
            io.readInto(d.data(), size);
            io.seek(oldPos);

            std::wstring wname = io::BinaryIO::stringToWString(name);
            Filesystem::windowsToUnixDelimiter(wname); // convert paths

            this->createFileRecursive(wname, d);
        }
        __debugbreak();
    }

    uint8_t* Archive::toData() const {
        const uint32_t fileSize = this->getSize();
        uint8_t* data = new uint8_t[fileSize];
        io::BinaryIO io(data);

        fs::Directory* root = getRoot();
        size_t count = root->getFileCount();

        uint32_t* offsetPositions = new uint32_t[count];
        uint32_t i = 0;

        io.writeBE<uint32_t>(count);
        std::stack<const fs::Directory*> stack;
        stack.push(root);

        while (!stack.empty()) {
            const fs::Directory* d = stack.top();
            stack.pop();

            for (const auto& [name, child] : d->getChildren()) {
                if (!child->isFile()) {
                    stack.push(dynamic_cast<const fs::Directory*>(child.get()));
                    continue;
                }

                std::wstring path = child->getPath().substr(1);
                Filesystem::unixToWindowsDelimiter(path);

                const fs::File* f = static_cast<const fs::File*>(child.get());
                io.writeBE<uint16_t>(path.length());
                io.writeUtf8(io::BinaryIO::wstringToString(path));
                // this stores the area where the file offset is written.
                offsetPositions[i] = io.getPosition();
                io.writeBE<uint32_t>(0);
                io.writeBE<uint32_t>(f->getSize());

                i++;
            }
        }

        uint32_t j = 0;

        stack = std::stack<const fs::Directory*>();
        stack.push(root);

        while (!stack.empty()) {
            const fs::Directory* d = stack.top();
            stack.pop();

            for (const auto& [name, child] : d->getChildren()) {
                if (!child->isFile()) {
                    stack.push(dynamic_cast<const fs::Directory*>(child.get()));
                    continue;
                }

                const fs::File* f = static_cast<const fs::File*>(child.get());
                // get current position (this is the position of the file)
                uint32_t pos = io.getPosition();
                // write the file
                io.writeBytes(f->getData().data(), f->getSize());
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
        }

        return io.getData();
    }

    uint64_t Archive::getSize() const {
        uint32_t size = 0;
        std::stack<const fs::Directory*> stack;
        stack.push(getRoot());

        while (!stack.empty()) {
            const fs::Directory* d = stack.top();
            stack.pop();

            for (const auto& [name, child] : d->getChildren()) {
                if (!child->isFile()) {
                    stack.push(dynamic_cast<const fs::Directory*>(child.get()));
                    continue;
                }

                std::wstring path = child->getPath().substr(1);
                Filesystem::unixToWindowsDelimiter(path);

                size += 2; // string length prefix
                size += path.length(); // name length
                size += 8; // offset, size
                size += dynamic_cast<const fs::File*>(child.get())->getSize(); // file size
            }
        }

        return size;
    }

} // namespace lce::arc
