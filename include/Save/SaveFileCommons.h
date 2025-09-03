//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef SAVEFILECOMMONS_H
#define SAVEFILECOMMONS_H
#include <Filesystem/Filesystem.h>
#include <IO/BinaryIO.h>
#include <libLCE.h>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace lce::save {
    class SaveFileOld;
    class SaveFile;

    enum SaveFileVersion : uint16_t { PR = 1, TU0054, TU5, TU9, TU14, TU17, TU19, TU36, TU69 };

    class LIBLCE_API SaveFileCommons : public fs::Filesystem {
    protected:
        static constexpr uint32_t HEADER_SIZE = 12;
        uint16_t originalVersion;
        uint16_t version;
        io::ByteOrder endian;

    public:
        SaveFileCommons() : endian(io::ByteOrder::LITTLE), originalVersion(11), version(11) {};

        virtual ~SaveFileCommons() = default;

        uint64_t getSize() const;

        uint32_t calculateIndexOffset() const;

        [[nodiscard]] virtual uint8_t* toData() const = 0;

        // TODO: this could probably be added to emscripten now
        static SaveFileCommons *readAuto(std::vector<uint8_t> data);
        static uint16_t getVersionFromData(std::vector<uint8_t> data, io::ByteOrder endian = io::ByteOrder::LITTLE);
        uint16_t getOriginalVersion() const;
        uint16_t getVersion() const;
        io::ByteOrder getEndian() const;
        void setOriginalVersion(uint16_t version);
        void setVersion(uint16_t version);
        void setEndian(io::ByteOrder endian);

        static io::ByteOrder detectEndian(std::vector<uint8_t> data);

        friend std::wostream& operator<<(std::wostream& wos, const SaveFileCommons &f) {
            wos << L"SaveFileCommons [" << L"indexOffset=" << f.calculateIndexOffset() << L", fileCount=" << f.getRoot()->getFileCount() << L", version=" << f.getVersion() << L", originalVersion=" << f.getOriginalVersion() << L"]";
            return wos;
        }

    protected:
        /**
         * Gets the size of an index entry based on the save file class type.
         * @return The size of an index entry
         */
        [[nodiscard]] virtual uint32_t getIndexEntrySize() const { return 0; };
    };
} // namespace lce::save


#endif // SAVEFILECOMMONS_H
