//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVEFILE_H
#define ARCHIVEFILE_H
#include <string>
#include "../../IO/BinaryIO.h"
#include "../../libLCE.h"
#include "../File.h"

// TODO: maybe create a universal type?

namespace lce::arc {
    class LIBLCE_API ArchiveInnerFile : File {
    public:
        std::string name;
        uint32_t size;
        uint32_t offset;
        uint8_t* data;

        ArchiveInnerFile();
        virtual ~ArchiveInnerFile();

        ArchiveInnerFile(uint8_t* data);

        ArchiveInnerFile(io::BinaryIO& io);
        
		uint32_t getSize() const override { return size; }

        uint8_t* create() const override;
    };
}


#endif
