//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "LCE/archive/Archive.h"

#include <BinaryIO/buffer/BinaryBuffer.h>
#include <BinaryIO/util/string/StringConverter.h>

namespace lce::arc {
    Archive::Archive(const Filesystem &fs) : Filesystem(fs) {};

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
