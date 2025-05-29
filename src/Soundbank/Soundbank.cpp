//
// Created by Boreal on 05/20/2025.
//

#include <IO/BinaryIO.h>
#include <Soundbank/Soundbank.h>

#include <filesystem>
#include <utility>

namespace lce::msscmp {

    Soundbank::Soundbank(uint8_t* data) {
        io::BinaryIO io(data);

        const std::string magic = io.readUtf8(4);

        byteOrder = magic == "BANK" ? io::ByteOrder::BIG : io::ByteOrder::LITTLE;

        io.seek(0x18);
        uint32_t firstVal = io.read<uint32_t>(byteOrder);
        uint32_t secondVal = io.read<uint32_t>(byteOrder);
        gen = (firstVal != secondVal) ? NEW_GEN : OLD_GEN; // if the two uint32_t's match, its old gen. idk why.
        // Dexrn: iirc this was just a difference that was consistent between the 2 so we can leverage it to figure out
        // which version it is

        io.seek(0x04); // go back

        io.read<uint32_t>(byteOrder); // Unknown "I forgot what this is"

        uint64_t dataStartOffset = readUintByGeneration(io, byteOrder, gen);

        io.read<uint64_t>(byteOrder); // Unknown

        uint64_t firstEntryOffset = readUintByGeneration(io, byteOrder, gen);

        uint64_t lastEntryOffset = readUintByGeneration(io, byteOrder, gen);

        io.read<uint64_t>(byteOrder); // Unknown

        if (gen == NEW_GEN)
            io.read<uint64_t>(byteOrder); // Unknown

        readUintByGeneration(io, byteOrder, gen);

        uint64_t index1Size = readUintByGeneration(io, byteOrder, gen);

        io.read<uint32_t>(byteOrder); // Unknown

        Generation opposite = (gen == NEW_GEN) ? OLD_GEN : NEW_GEN;
        index2Size = readUintByGeneration(io, byteOrder, opposite);

        std::string name = io.readUtf8(12);

        io.seek(lastEntryOffset + 4);

        for (uint64_t i = 0; i < index2Size; i++) {
            io.seek((lastEntryOffset + 4) + (i * 8));

            uint32_t entryOffset = io.read<uint32_t>(byteOrder);

            uint32_t fStructureOffset = 0;
            if (i != index2Size) {
                fStructureOffset = io.read<uint32_t>(byteOrder);
            }

            io.seek(entryOffset);

            uint32_t nameOffset = io.read<uint32_t>(byteOrder);
            uint32_t currentOffset = io.getPosition();

            io.seek(nameOffset);
            std::string fileName = io.readUtf8NullTerminated();
            io.seek(currentOffset);

            io.read<uint32_t>(byteOrder);

            uint32_t dataOffset = io.read<uint32_t>(io::ByteOrder::LITTLE);
            io.read<uint32_t>(byteOrder);
            io.read<uint32_t>(byteOrder);
            uint32_t sampleRate = io.read<uint32_t>(byteOrder);
            uint32_t fileSize = io.read<uint32_t>(byteOrder);

            int32_t oldPos = io.getPosition();

            std::vector<uint8_t> d;
            d.resize(fileSize);

            io.seek(dataOffset);
            io.readInto(d.data(), fileSize);
            io.seek(oldPos);

            std::wstring wname = io::BinaryIO::stringToWString(fileName);

            std::filesystem::path path(
                wname); // TODO: I forgot about this, we could use this I think inside the FS class

            std::unique_ptr<BinkaFile> f =
                std::make_unique<BinkaFile>(path.filename().wstring() + L".binka", d, sampleRate, nullptr);
            fs::Directory* dir = getOrCreateDirByPath(path.parent_path().wstring());

            dir->addChild(std::move(f));
        }
    }
} // namespace lce::msscmp
