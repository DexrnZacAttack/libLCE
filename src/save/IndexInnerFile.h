//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef INNERFILE_H
#define INNERFILE_H
#include <string>

#include "../io/ByteOrder.h"

namespace lce::save {
    // wish I could just dump a struct lol
    class IndexInnerFile {
    public:
        // should use wchar_t name[64] I think?
        std::wstring name;
        uint32_t size{};
        mutable uint32_t offset{};
        mutable uint64_t timestamp{};
        uint8_t* data{};
        ByteOrder endian;

        IndexInnerFile();

        explicit IndexInnerFile(uint8_t *data, bool readOld = false, ByteOrder endian = LITTLE);

    };
}

#endif //INNERFILE_H
