//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef SAVEFILE_H
#define SAVEFILE_H
#include <cstdint>
#include <string>
#include <vector>

#include "IndexInnerFile.h"
#include "SaveFileCommons.h"
#include "../IO/ByteEnums.h"

namespace lce::save {

class LIBLCE_API SaveFile : public SaveFileCommons {
public:
    SaveFile(uint32_t indexOffset, uint32_t indexFileCount, uint16_t origVersion, uint16_t version, const std::vector<std::shared_ptr<IndexInnerFile>> &index);
    SaveFile();
    explicit SaveFile(ByteOrder endian);

    explicit SaveFile(std::vector<uint8_t> data, ByteOrder endian = LITTLE);

    uint8_t* create() const override;

    protected:
        uint32_t getIndexEntrySize() const override;
};

} // lce::save

#endif //SAVEFILE_H
