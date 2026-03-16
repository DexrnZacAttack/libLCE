//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVE_H
#define ARCHIVE_H
#include "../../../../libBIO/include/BinaryIO/io/interface/IReadable.h"
#include "../../../../libBIO/include/BinaryIO/io/interface/IWritable.h"
#include "BinaryIO/container/FixedArray.h"
#include "LCE/io/Serializable.h"
#include "BinaryIO/io/Serializable.h"
#include "BinaryIO/util/string/StringConverter.h"

#include <vector>

#include "LCE/filesystem/Filesystem.h"
#include "LCE/libLCE.h"

namespace lce::arc {
    /**
     * LCE Archive (.ARC) File
     * \brief A standard archive that can store files,
     * commonly used for storing game assets.
     *
     * ## Header
     * - File Count (`uint32_t`)
     *
     * ## File Index
     * For each file,
     * - Name Size (`uint16_t`)
     * - Name (`Name Size` bytes)
     * - File Offset (`uint32_t`)
     *   - Offset to the file data
     * - File Size (`uint32_t`)
     *
     * ## File Data
     * For each file,
     * - File Data (`File Size` bytes)
     */
    class LIBLCE_API Archive final : public fs::Filesystem,
                                     public io::Serializable {
      public:
        using Serializer = bio::io::Serializable<lce::arc::Archive, void>;
        using Deserializer = bio::io::Deserializable<lce::arc::Archive, void>;

        struct FileEntry {
            std::string name = "";
            uint32_t offset = 0;
            uint32_t size = 0;
        };

        /** Creates an empty archive file */
        Archive() = default;
        /** Creates an archive file with the contents of a physical folder */
        explicit Archive(const Filesystem &fs);

        /** Calculates the total size of the archive file in bytes
         *
         * @returns The archive file size in bytes
         */
        size_t getSize() const override;

        friend std::wostream &operator<<(std::wostream &wos, const Archive &a) {
            wos << L"Archive[" << L"fileCount=" << a.getRoot()->getFileCount()
                << L"]";
            return wos;
        }
    };
} // namespace lce::arc

template<>
class bio::io::Serializable<lce::arc::Archive, void> {
public:
    using Type = lce::arc::Archive;
    using Options = void;

    static void serialize(const lce::arc::Archive& value, WritableBufferLike auto &writable) {
        const lce::fs::Directory *root = value.getRoot();

        const size_t count = root->getFileCount();
        writable.template writeBE<uint32_t>(count);

        uint32_t *offsetPositions = new uint32_t[count];

        uint32_t i = 0;
        root->forEachFilesRecursive(
            [&i, &writable, &offsetPositions](const std::wstring &n,
                                        const lce::fs::File &f) {
                std::wstring path = f.getPath().substr(1);
                lce::fs::Filesystem::unixToWindowsDelimiter(path);

                writable.template writeString<char>(bio::util::string::StringConverter::wstringToString(path), util::ByteOrder::BIG, util::string::StringLengthEncoding::LENGTH_PREFIX);

                // this stores the area where the file offset is written.
                offsetPositions[i] = writable.getOffset();
                writable.template writeBE<uint32_t>(0);
                writable.template writeBE<uint32_t>(f.getSize());

                i++;
            });

        uint32_t j = 0;
        root->forEachFilesRecursive(
            [&j, &writable, &offsetPositions](const std::wstring &n,
                                        const lce::fs::File &f) {
                // get current position (this is the position of the file)
                const uint32_t fPos = writable.getOffset();
                // write the file
                writable.writeBytes(f.getData().data(), f.getSize());

                // get the position after the file was written (we return here
                // to write the next one)
                const uint32_t last = writable.getOffset();

                // go to the offset's offset (lol) and write the actual offset.
                writable.seek(offsetPositions[j]);
                writable.template writeBE<uint32_t>(fPos);

                // seek back over to the next file's position.
                writable.seek(last);

                // and then we increment this.
                j++;
            });
    }
};

template <>
class bio::io::Deserializable<lce::arc::Archive, void> {
public:
    using Type = lce::arc::Archive;
    using Options = void;

    static std::unique_ptr<lce::arc::Archive> deserialize(ReadableBufferLike auto &readable) {
        std::unique_ptr<lce::arc::Archive> arc(new lce::arc::Archive());
        const uint32_t fileCount = readable.template readBE<uint32_t>();

        container::FixedArray<lce::arc::Archive::FileEntry> table(fileCount);

        // read into table
        for (auto &[name, offset, size] : table) {
            name = readable.template readStringWithLength<char>(util::ByteOrder::BIG, util::string::StringLengthEncoding::LENGTH_PREFIX);
            offset = readable.template readBE<uint32_t>();
            size = readable.template readBE<uint32_t>();
        }

        // read files in table
        for (const auto &[name, offset, size] : table) {
            readable.seek(offset);
            std::vector<uint8_t> d = readable.readOfSizeVec(size);

            arc->createFileRecursive(lce::fs::Filesystem::windowsToUnixDelimiter(
            bio::util::string::StringConverter::stringToWString(name)
            ), std::move(d));
        }

        return std::move(arc);
    }
};

#endif // ARCHIVE_H
