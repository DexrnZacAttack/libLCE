//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef SAVEFILECOMMONS_H
#define SAVEFILECOMMONS_H
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include "../IO/ByteEnums.h"
#include "../libLCE.h"
#include "../Filesystem/Filesystem.h"

namespace lce::save {
    class SaveFileOld;
    class SaveFile;
    class IndexInnerFile;

    enum SaveFileVersion : uint16_t {
        PR = 1,
        TU0054,
        TU5,
        TU9,
        TU14,
        TU17,
        TU19,
        TU36,
        TU69
    };

    class LIBLCE_API SaveFileCommons : public fs::Filesystem {
        protected:
        static constexpr uint32_t HEADER_SIZE = 12;
        uint32_t indexOffset;
        uint16_t originalVersion;
        uint16_t version;
        ByteOrder endian;
        public:
            SaveFileCommons(): endian(ByteOrder::LITTLE), indexOffset(0), originalVersion(11), version(11) {};

            virtual ~SaveFileCommons() = default;

            uint64_t getSize() const override;

            uint32_t calculateIndexOffset() const;

            [[nodiscard]] uint32_t getFilesSize() const;

#ifndef __EMSCRIPTEN__
            static std::variant<SaveFile, SaveFileOld> readAuto(std::vector<uint8_t> data, ByteOrder endian = LITTLE);
#endif
            static uint16_t getVersionFromData(std::vector<uint8_t> data, ByteOrder endian = LITTLE);
            uint32_t getIndexOffset() const;
            uint16_t getOriginalVersion() const;
            uint16_t getVersion() const;
            ByteOrder getEndian() const;   
            void setOriginalVersion(uint16_t version);
            void setVersion(uint16_t version);
            void setEndian(ByteOrder endian);
            
            uint8_t* create() const override;
        protected:
            virtual uint32_t getIndexEntrySize() const;
    };
}


#endif //SAVEFILECOMMONS_H
