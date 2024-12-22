//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <vector>
#include "ArchiveFile.h"

namespace lce::arc {
    class Archive {
    public:
        uint32_t fileCount;
        std::vector<ArchiveFile> index;

        Archive(uint32_t fileCount, const std::vector<ArchiveFile> &index);
        Archive();

        static Archive readFromData(uint8_t* data);
    };
}


#endif //ARCHIVE_H
