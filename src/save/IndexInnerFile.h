//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef INNERFILE_H
#define INNERFILE_H
#include <string>

namespace lce::save {
    class IndexInnerFile {
    public:
        std::wstring name;
        uint32_t size;
        mutable uint32_t offset;
        mutable uint64_t timestamp;
        uint8_t* data;

        IndexInnerFile();
        ~IndexInnerFile();

        static IndexInnerFile readInnerFile(uint8_t* data);

        static IndexInnerFile readOldInnerFile(uint8_t *data);
    };
}


#endif //INNERFILE_H
