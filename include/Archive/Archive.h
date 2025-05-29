//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <vector>
#include <Filesystem/Filesystem.h>
#include <libLCE.h>


namespace lce::arc {

    class LIBLCE_API Archive : public fs::Filesystem {
    public:
        uint32_t fileCount;

        Archive(uint32_t fileCount, std::vector<std::shared_ptr<fs::File>> &index);
        Archive();

        Archive(uint8_t* data);
        uint64_t getSize() const;

        uint8_t* toData() const;
    };
}


#endif //ARCHIVE_H
