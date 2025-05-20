//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <vector>
#include "../File/InnerFile.h"
#include "../libLCE.h"


namespace lce::arc {
    class LIBLCE_API Archive : public file::File {
    public:
        uint32_t fileCount;
        std::vector<file::InnerFile> index;

        Archive(uint32_t fileCount, const std::vector<file::InnerFile> &index);
        Archive();

        Archive(uint8_t* data);

        uint8_t* create() const override;

        uint32_t getSize() const override;

        void addFile(file::InnerFile file);

        void removeFile(uint32_t index);
    };
}


#endif //ARCHIVE_H
