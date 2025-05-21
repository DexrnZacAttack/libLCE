//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef INNERFILE_H
#define INNERFILE_H
#include <string>
#include <vector>

#include "../IO/ByteEnums.h"
#include "../libLCE.h"
#include "../Filesystem/File.h"

namespace lce::save {
    // wish I could just dump a struct lol
    class LIBLCE_API IndexInnerFile : public fs::File {
    protected:
        mutable uint64_t timestamp{};
        ByteOrder endian;
    public:
        IndexInnerFile();

        explicit IndexInnerFile(uint8_t *data, bool readOld = false, ByteOrder endian = LITTLE);

        uint64_t getTimestamp() const;
        void setTimestamp(uint64_t timestamp);
        
        ByteOrder getEndian() const;
        void setEndian(ByteOrder endian);
    };
}

#endif //INNERFILE_H
