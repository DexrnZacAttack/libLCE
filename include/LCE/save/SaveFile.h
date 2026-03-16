//
// Created by DexrnZacAttack on 12/21/2024.
//

#ifndef AbstractSaveFile_H
#define AbstractSaveFile_H
#include "BinaryIO/buffer/SizedBinaryBuffer.h"
#include "BinaryIO/util/string/StringConverter.h"
#include "LCE/io/Serializable.h"

#include "LCE/filesystem/Filesystem.h"
#include "LCE/libLCE.h"
#include <BinaryIO/buffer/BinaryBuffer.h>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "LCE/filesystem/Directory.h"
#include "LCE/filesystem/File.h"
#include "LCE/util/StringUtilities.h"

namespace lce::save {

    /** Save file versions, each value's name denotes the first TU it was used
     * in
     *
     * @sa @link https://team-lodestone.github.io/Documentation/LCE/Saving/LCE%20Save%20File%20Versions @endlink
     * for more info
     */
    enum SaveFileVersion : uint16_t {
        B0033 = 1, /**< Pre-release builds, Builds 0033-0035 */
        B0054,     /**< Used between pre-release build 0054 and TU 4 */
        TU5,       /**< Used between TUs 5-8  */
        TU9,       /**< Used between TUs 9-13  */
        TU14,      /**< Used between TUs 14-16  */
        TU17,      /**< Used between TUs 17-18  */
        TU19,      /**< Used between TUs 19-35  */
        TU36,      /**< Used between TUs 36-68  */
        TU69       /**< Used on TU 69 onwards  */
    };

    /** Represents a Save file (aka savegame.dat, GAMEDATA, etc.)
     *
     * @sa @link https://team-lodestone.github.io/Documentation/LCE/Saving/Save%20Format "The save file format structure"
     * @sa @link https://team-lodestone.github.io/Documentation/LCE/Saving/Pre-Release%20Save%20Format "The pre-release save file format structure (B0033)"
     */
    class LIBLCE_API SaveFile : public fs::Filesystem {
    public:
        /** Creates a save file with the contents of the given Filesystem */
        SaveFile(const Filesystem &fs,
                        const uint16_t origVersion = 11,
                        const uint16_t version = 11)
            : Filesystem(fs), m_originalVersion(origVersion), m_version(version)
               {};

        explicit SaveFile(
            const uint16_t origVersion = 11, const uint16_t version = 11)
            : m_originalVersion(origVersion), m_version(version)
               {};

        struct SerializerOptions {
            bio::util::ByteOrder byteOrder;
        };

        static constexpr int FILE_ENTRY_SIZE = 144;
        static constexpr int OLD_FILE_ENTRY_SIZE = 136;

        struct FileEntry {
            std::u16string name = u"";
            uint32_t size = 0;
            uint32_t offset = 0;
            uint64_t timestamp = 0;
        };

        using Serializer = bio::io::Serializable<SaveFile, SerializerOptions>;
        using Deserializer = bio::io::Deserializable<SaveFile, SerializerOptions>;

        /** Header size in bytes of the file once serialized
         * - Index Offset (`uint32_t`) -> 4 bytes
         * - File Count (`uint32_t`) -> 4 bytes
         * - Original Version (`uint16_t`) -> 2 bytes
         * - Version (`uint16_t`) -> 2 bytes
         */
        static constexpr uint32_t HEADER_SIZE = 12;

        /** Calculates the total size of the save file in bytes
         *
         * @returns The save file size in bytes
         */
        virtual size_t getSize() const;

        /** Calculates the offset where the index shall be placed, which is
         * always after the header and all the file data
         *
         * @returns The index offset
         */
        [[nodiscard]] uint32_t calculateIndexOffset() const;

        /** @returns The save file without requiring endian and version */
        static SaveFile *deserializeAuto(std::vector<uint8_t> &data);

        /** @returns The serialized save file's version */
        static uint16_t getVersionFromData(
            std::vector<uint8_t> &data,
            bio::util::ByteOrder byteOrder = bio::util::ByteOrder::LITTLE);

        /** @returns The save file's original version */
        uint16_t getOriginalVersion() const;

        /** @returns The save file's version */
        uint16_t getVersion() const;

        /** Sets the save file's original version */
        void setOriginalVersion(uint16_t version);

        /** Sets the save file's version */
        void setVersion(uint16_t version);

        size_t getIndexEntrySize() const;

        friend std::wostream &operator<<(std::wostream &wos,
                                         const SaveFile &f) {
            wos << L"AbstractSaveFile [" << L"indexOffset="
                << f.calculateIndexOffset() << L", fileCount="
                << f.getRoot()->getFileCount() << L", version="
                << f.getVersion() << L", originalVersion="
                << f.getOriginalVersion() << L"]";
            return wos;
        }

      protected:
        uint16_t m_originalVersion;
        uint16_t m_version;
    };
} // namespace lce::save

/** Serializer for the Save file (aka savegame.dat, GAMEDATA, etc.)
 *
 * @note If the save file's version is @ref lce::save::SaveFileVersion::B0033 , then the file format is written out slightly differently.
 *
 * @sa @link https://team-lodestone.github.io/Documentation/LCE/Saving/Save%20Format "The save file format structure"
 * @sa @link https://team-lodestone.github.io/Documentation/LCE/Saving/Pre-Release%20Save%20Format "The pre-release save file format structure (B0033)"
 */
template<>
class bio::io::Serializable<lce::save::SaveFile, lce::save::SaveFile::SerializerOptions> {
public:
    using Type = lce::save::SaveFile;
    using Options = lce::save::SaveFile::SerializerOptions;

    static void serialize(const lce::save::SaveFile &saveFile, WritableBufferLike auto &writable, const lce::save::SaveFile::SerializerOptions &options) {
        const lce::fs::Directory *root = saveFile.getRoot();

        if (saveFile.calculateIndexOffset() > 0xFFFFFFFF - lce::save::SaveFile::HEADER_SIZE)
            throw std::out_of_range("Index offset is too large to be stored in a 32-bit integer");

        uint32_t fileCount = root->getFileCount();
        if (saveFile.getVersion() <= lce::save::SaveFileVersion::B0033) {
            fileCount *= lce::save::SaveFile::OLD_FILE_ENTRY_SIZE;
        }

        //write header
        writable.template write<uint32_t>(0, options.byteOrder);
        writable.template write<uint32_t>(fileCount, options.byteOrder);
        writable.template write<uint16_t>(saveFile.getOriginalVersion(), options.byteOrder);
        writable.template write<uint16_t>(saveFile.getVersion(), options.byteOrder);

        //store each file we've written, we write the index entries after
        std::vector<lce::save::SaveFile::FileEntry> written;

        // write data for each file
        root->forEachFilesRecursive([&writable, &written](const std::wstring &name, const lce::fs::File &innerFile) {
            writable.writeBytes(innerFile.getData().data(), innerFile.getSize());

            //push into the vector
            written.push_back(lce::save::SaveFile::FileEntry {
                bio::util::string::StringConverter::wstringToU16string(innerFile.getPath().substr(1)),
                static_cast<uint32_t>(innerFile.getSize()),
                static_cast<uint32_t>(writable.getOffset()),
                innerFile.getModifiedTimestamp()
            });
        });

        //we have written all the files, so the index starts here.
        uint32_t indexOffset = writable.getOffset();

        //write each file
        for (auto &[name, size, offset, timestamp] : written) {
            name.resize(64);
            writable.template writeString<char16_t>(name, options.byteOrder, bio::util::string::StringLengthEncoding::NONE);
            writable.template write<uint32_t>(size, options.byteOrder);
            writable.template write<uint32_t>(offset, options.byteOrder);

            if (saveFile.getVersion() > lce::save::SaveFileVersion::B0033) {
                writable.template write<uint64_t>(timestamp, options.byteOrder);
            }
        }

        //write the index offset
        writable.seek(0);
        writable.template write<uint32_t>(indexOffset, options.byteOrder);
    }
};


/** Deserializer for the Save file (aka savegame.dat, GAMEDATA, etc.)
 *
 * @note If the save file's version is @ref lce::save::SaveFileVersion::B0033 , then the file format is read in slightly differently.
 *
 * @sa @link https://team-lodestone.github.io/Documentation/LCE/Saving/Save%20Format "The save file format structure"
 * @sa @link https://team-lodestone.github.io/Documentation/LCE/Saving/Pre-Release%20Save%20Format "The pre-release save file format structure (B0033)"
 */
template <>
class bio::io::Deserializable<lce::save::SaveFile, lce::save::SaveFile::SerializerOptions> {
public:
    using Type = lce::save::SaveFile;
    using Options = lce::save::SaveFile::SerializerOptions;

    /** Detects the save file's byte order based off of serialized data
     *
     * @returns The save file's byte order (endianness)
     */
    static bio::util::ByteOrder detectByteOrder(ReadableBufferLike auto &readable) {
        readable.seek(4 + 4 + 2);

        const uint8_t *s = readable.position();

        return *s != 0x00 ? bio::util::ByteOrder::LITTLE : bio::util::ByteOrder::BIG;
    }

    static std::unique_ptr<lce::save::SaveFile> deserialize(ReadableBufferLike auto &readable, const lce::save::SaveFile::SerializerOptions &options) {
        const uint32_t indexOffset = readable.template read<uint32_t>(options.byteOrder);

        //if the user provides a buffer that we know the size of, we can do a bounds check
        // TODO in BIO I can make ISized for this
        if (const buffer::SizedBinaryBuffer *buf = dynamic_cast<buffer::SizedBinaryBuffer *>(&readable); buf != nullptr) {
            if (indexOffset > buf->size()) {
                throw std::out_of_range("Index offset points out of bounds");
            }
        }

        uint32_t fileCount = readable.template read<uint32_t>(options.byteOrder);

        lce::save::SaveFileVersion originalVersion = readable.template read<lce::save::SaveFileVersion>(options.byteOrder);
        lce::save::SaveFileVersion version = readable.template read<lce::save::SaveFileVersion>(options.byteOrder);

        if (version <= lce::save::B0033) {
            fileCount /= lce::save::SaveFile::OLD_FILE_ENTRY_SIZE;
        }

        std::unique_ptr<lce::save::SaveFile> save = std::make_unique<lce::save::SaveFile>(originalVersion, version);

        if (fileCount > (0xFFFFFFFF - lce::save::SaveFile::HEADER_SIZE) / lce::save::SaveFile::FILE_ENTRY_SIZE)
            throw std::out_of_range("File count (" +
                                     std::to_string(fileCount) +
                                     ") makes the file too big for it's index "
                                     "offset to stored in a 32-bit integer.");

        container::FixedArray<lce::save::SaveFile::FileEntry> table(fileCount);

        // go to the table
        readable.seek(indexOffset);

        //read the table
        for (auto &[name, size, offset, timestamp] : table) {
            name = lce::util::StringUtilities::trimEndNullBytes<char16_t>(
                readable.template readString<char16_t>(64, options.byteOrder)
            );

            size = readable.template read<uint32_t>(options.byteOrder);
            offset = readable.template read<uint32_t>(options.byteOrder);

            if (version > lce::save::B0033) {
                timestamp = readable.template read<uint64_t>(options.byteOrder);
            }
        }

        //read the data
        for (const auto &file : table) {
            readable.seek(file.offset);

            std::vector<uint8_t> data = readable.readOfSizeVec(file.size);

            // create the file
            (void)save->createFileRecursive(util::string::StringConverter::u16stringToWstring(file.name), std::move(data));
        }

        return std::move(save);
    }
};

#endif // AbstractSaveFile_H
