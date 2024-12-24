//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <vector>
#include "ArchiveInnerFile.h"

namespace lce::arc {
    class Archive {
    public:
        uint32_t fileCount;
        std::vector<ArchiveInnerFile> index;

        Archive(uint32_t fileCount, const std::vector<ArchiveInnerFile> &index);
        Archive();

        static Archive readFromData(uint8_t* data);

        uint8_t *create();

        uint32_t getSize() const;

        void addFile(ArchiveInnerFile file);

        void removeFile(uint32_t index);
    };
}


#endif //ARCHIVE_H
