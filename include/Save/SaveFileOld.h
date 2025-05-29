//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef OLDSAVEFILE_H
#define OLDSAVEFILE_H
#include <Save/SaveFile.h>
#include <Save/SaveFileCommons.h>

namespace lce::save {
    class LIBLCE_API SaveFileOld : public SaveFileCommons {
    public:
        explicit SaveFileOld(ByteOrder endian);
        explicit SaveFileOld(std::vector<uint8_t> data, ByteOrder endian = BIG); // big endian cuz xb360 was the only edition with this format

        SaveFile* upgrade(uint16_t version);

        [[nodiscard]] uint8_t* toData() const override;
    
    protected:
        /**
         * Gets the size of an index entry based on the save file class type.
         * @return The size of an index entry
         */
		[[nodiscard]] uint32_t getIndexEntrySize() const override { return 136; };
    };
}


#endif //OLDSAVEFILE_H
