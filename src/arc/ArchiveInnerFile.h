//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVEFILE_H
#define ARCHIVEFILE_H
#include <string>
#include "../io/BinaryIO.h"

// TODO: maybe create a universal type?

namespace lce::arc {
    class ArchiveInnerFile {
    public:
        std::string name;
        uint32_t size;
        uint32_t offset;
        uint8_t* data;

        ArchiveInnerFile();
        ~ArchiveInnerFile();

        static ArchiveInnerFile readArchiveFile(uint8_t* data);

        static ArchiveInnerFile readArchiveFileBIO(io::BinaryIO& io);

        uint8_t *create();
    };
}


#endif
