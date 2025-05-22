//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef OLDSAVEFILE_H
#define OLDSAVEFILE_H
#include "SaveFile.h"
#include "SaveFileCommons.h"

namespace lce::save {
    class LIBLCE_API SaveFileOld : public lce::save::SaveFileCommons {
    public:
        explicit SaveFileOld(ByteOrder endian);
        explicit SaveFileOld(std::vector<uint8_t> data, ByteOrder endian = BIG); // big endian cuz xb360 was the only edition with this format

        uint8_t* create() const override;

        SaveFile* migrate(uint16_t version);
    
    protected:
		uint32_t getIndexEntrySize() const override;
    };
}


#endif //OLDSAVEFILE_H
