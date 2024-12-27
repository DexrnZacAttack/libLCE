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

namespace lce::save {

class SaveFile : public SaveFileCommons {
public:
    SaveFile(uint32_t indexOffset, uint32_t indexFileCount, uint16_t origVersion, uint16_t version, const std::vector<IndexInnerFile> &index);
    SaveFile();

    static SaveFile read(std::vector<uint8_t> data);

    const uint8_t *create();

    protected:
        uint32_t getIndexEntrySize() override;
};

} // lce::save

#endif //SAVEFILE_H
