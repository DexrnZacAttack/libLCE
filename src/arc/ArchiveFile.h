//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVEFILE_H
#define ARCHIVEFILE_H
#include <string>
#include "../io/BinaryIO.h"

// TODO: maybe create a universal type?

namespace lce::arc {
    class ArchiveFile {
    public:
        std::string name;
        uint32_t size;
        uint32_t offset;
        uint8_t* data;

        ArchiveFile();
        ~ArchiveFile();

        static ArchiveFile readArchiveFile(uint8_t* data);

        static ArchiveFile readArchiveFileBIO(io::BinaryIO& io);
    };
}


#endif
