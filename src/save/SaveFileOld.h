//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef OLDSAVEFILE_H
#define OLDSAVEFILE_H
#include "SaveFile.h"
#include "SaveFileCommons.h"

namespace lce::save {
    class SaveFileOld : public lce::save::SaveFileCommons {
    public:
        static SaveFileOld read(std::vector<uint8_t> data);

        const uint8_t *create();

        SaveFile migrate(uint16_t version);

    protected:
        uint32_t getIndexEntrySize() override;
    };
}


#endif //OLDSAVEFILE_H
