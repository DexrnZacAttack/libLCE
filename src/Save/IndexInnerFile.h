//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef INNERFILE_H
#define INNERFILE_H
#include <string>
#include <vector>

#include "../IO/ByteOrder.h"
#include "../lce.h"

namespace lce::save {
    // wish I could just dump a struct lol
    class LIBLCE_API IndexInnerFile {
    protected:
        // save file file name is ALWAYS 2 bytes
        std::u16string name;
        uint32_t size{};
        mutable uint32_t offset{};
        mutable uint64_t timestamp{};
        uint8_t* data{};
        ByteOrder endian;
    public:
        IndexInnerFile();

        explicit IndexInnerFile(uint8_t *data, bool readOld = false, ByteOrder endian = LITTLE);

        std::u16string getName() const;
        void setName(const std::u16string &name);
        uint32_t getSize() const;
        uint32_t getOffset() const;
        void setOffset(uint32_t offset);
        uint64_t getTimestamp() const;
        void setTimestamp(uint64_t timestamp);
        uint8_t* getData() const;

        std::vector<uint8_t> getDataVec() const;

        void setDataVec(std::vector<uint8_t> vector);

        void setData(uint8_t *data);
        ByteOrder getEndian() const;
        void setEndian(ByteOrder endian);
    };
}

#endif //INNERFILE_H
