//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <vector>
#include "../Filesystem/File.h"
#include "../libLCE.h"


namespace lce::arc {
    class LIBLCE_API Archive : public fs::Filesystem {
    public:
        uint32_t fileCount;
        std::vector<fs::File> index;

        Archive(uint32_t fileCount, const std::vector<fs::File> &index);
        Archive();

        Archive(uint8_t* data);

        uint8_t* create() const override;

        uint64_t getSize() const override;

        void addFile(fs::File file);

        void removeFile(uint32_t index);
    };
}


#endif //ARCHIVE_H
