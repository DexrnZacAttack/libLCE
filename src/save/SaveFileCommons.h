//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef SAVEFILECOMMONS_H
#define SAVEFILECOMMONS_H
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include "../io/ByteOrder.h"

// TODO: allow for allocating/freeing space when needed

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

    class SaveFileCommons {
        protected:
        const uint32_t HEADER_SIZE = 12;
        size_t wcharSize = sizeof(wchar_t);
        uint32_t indexOffset;
        uint32_t indexFileCount;
        uint16_t originalVersion;
        uint16_t version;
        ByteOrder endian;
        public:
            SaveFileCommons(): endian(ByteOrder::LITTLE), indexOffset(0), indexFileCount(0), originalVersion(11), version(11) {};

            virtual ~SaveFileCommons() = default;

            std::vector<IndexInnerFile> index;

            void addFile(const IndexInnerFile &file);

            void removeFile(uint32_t index);

            uint32_t getSize();

            std::optional<IndexInnerFile> getFileByName(std::wstring name);

            uint32_t calculateIndexOffset() const;

            [[nodiscard]] uint32_t getFilesSize() const;

            static std::variant<SaveFile, SaveFileOld> readAuto(std::vector<uint8_t> data);

            uint32_t getFileCount() const;
            uint16_t getOriginalVersion() const;
            uint16_t getVersion() const;
            ByteOrder getEndian() const;

            void setOriginalVersion(uint16_t version);
            void setVersion(uint16_t version);
            void setEndian(ByteOrder endian);
        protected:
            virtual uint32_t getIndexEntrySize();
    };
}


#endif //SAVEFILECOMMONS_H
