//
// Created by Boreal on 05/20/2025.
//

#include <IO/BinaryIO.h>
#include <Soundbank/Soundbank.h>

#include <filesystem>
#include <utility>

namespace lce::msscmp {
    Soundbank::Soundbank(uint8_t *data) {
        io::BinaryIO io(data);

        const std::string magic = io.readUtf8(4);

        mByteOrder =
            magic == "BANK" ? io::ByteOrder::BIG : io::ByteOrder::LITTLE;

        io.seek(0x18);
        const uint32_t firstVal = io.read<uint32_t>(mByteOrder);
        const uint32_t secondVal = io.read<uint32_t>(mByteOrder);
        mType =
            (firstVal != secondVal)
                ? NEW_GEN
                : OLD_GEN; // if the two uint32_t's match, its old gen. idk why.
        // Dexrn: iirc this was just a difference that was consistent between
        // the 2 so we can leverage it to figure out which version it is

        io.seek(0x04); // go back

        io.read<uint32_t>(mByteOrder); // Unknown "I forgot what this is"

        uint64_t dataStartOffset = readUintByType(io, mByteOrder, mType);

        io.read<uint64_t>(mByteOrder); // Unknown

        uint64_t firstEntryOffset = readUintByType(io, mByteOrder, mType);

        const uint64_t lastEntryOffset = readUintByType(io, mByteOrder, mType);

        io.read<uint64_t>(mByteOrder); // Unknown

        if (mType == NEW_GEN)
            io.read<uint64_t>(mByteOrder); // Unknown

        readUintByType(io, mByteOrder, mType);

        uint64_t index1Size = readUintByType(io, mByteOrder, mType);

        io.read<uint32_t>(mByteOrder); // Unknown

        const Type opposite = (mType == NEW_GEN) ? OLD_GEN : NEW_GEN;
        mIndex2Size = readUintByType(io, mByteOrder, opposite);

        std::string name = io.readUtf8(12);

        io.seek(lastEntryOffset + 4);

        for (uint64_t i = 0; i < mIndex2Size; i++) {
            io.seek((lastEntryOffset + 4) + (i * 8));

            const uint32_t entryOffset = io.read<uint32_t>(mByteOrder);

            uint32_t fStructureOffset = 0;
            if (i != mIndex2Size) {
                fStructureOffset = io.read<uint32_t>(mByteOrder);
            }

            io.seek(entryOffset);

            const uint32_t nameOffset = io.read<uint32_t>(mByteOrder);
            const uint32_t currentOffset = io.getPosition();

            io.seek(nameOffset);
            std::string fileName = io.readUtf8NullTerminated();
            io.seek(currentOffset);

            io.read<uint32_t>(mByteOrder);

            const uint32_t dataOffset =
                io.read<uint32_t>(io::ByteOrder::LITTLE);
            io.read<uint32_t>(mByteOrder);
            io.read<uint32_t>(mByteOrder);
            uint32_t sampleRate = io.read<uint32_t>(mByteOrder);
            const uint32_t fileSize = io.read<uint32_t>(mByteOrder);

            const size_t oldPos = io.getPosition();

            std::vector<uint8_t> d;
            d.resize(fileSize);

            io.seek(dataOffset);
            io.readInto(d.data(), fileSize);
            io.seek(oldPos);

            std::wstring wname = io::BinaryIO::stringToWString(fileName);

            std::filesystem::path path(
                wname); // TODO: I forgot about this, we could use this I think
                        // inside the FS class

            std::unique_ptr<BinkaFile> f = std::make_unique<BinkaFile>(
                path.filename().wstring() + L".binka", d, sampleRate, nullptr);
            fs::Directory *dir =
                getOrCreateDirByPath(path.parent_path().wstring());

            dir->addChild(std::move(f));
        }
    }

    Soundbank::Soundbank(std::vector<uint8_t> data) : Soundbank(data.data()) {
    }

    bool Soundbank::isSoundbank(uint8_t *data) {
        io::BinaryIO io(data);
        const std::string magic = io.readUtf8(4);

        return magic == "BANK" || magic == "KNAB";
    }
} // namespace lce::msscmp
