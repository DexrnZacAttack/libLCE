//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVEFILE_H
#define ARCHIVEFILE_H
#include <string>
#include "../IO/BinaryIO.h"
#include "../libLCE.h"
#include "File.h"

namespace lce::file {
    class LIBLCE_API InnerFile : File {
    public:
        std::string name;
        uint32_t size;
        uint32_t offset;
        uint8_t* data;

        InnerFile();
        virtual ~InnerFile();

        InnerFile(uint8_t* data);

		uint32_t getSize() const override { return size; }

        uint8_t* create() const override;
    };
}


#endif
