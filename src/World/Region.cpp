//
// Created by DexrnZacAttack on 12/28/2024.
//

#include <regex>

#include <World/Chunk.h>
#include <World/Region.h>

namespace lce::world {
    Region::Region(int16_t x, int16_t z, int16_t dim) : x(x), z(z), dim(dim) {}

    Region::Region(std::vector<uint8_t> data, int16_t x, int16_t z, int16_t dim,
                   compression::CompressionType outerCompression,
                   io::ByteOrder byteOrder)
        : x(x), z(z), dim(dim) {}

    Region::Region(std::wstring filename) {}

    Region::Region(std::vector<uint8_t> data, std::wstring filename,
                   compression::CompressionType outerCompression,
                   io::ByteOrder byteOrder) {
        const std::wregex re(
            L"(DIM([-0-9]{1,2}))?(r)\\.([-0-9]{1,2})\\.([-0-9]{1,2}).mcr");
        std::wsmatch match;

        if (std::regex_match(filename, match, re)) {
            try {
                this->dim = std::stoi(match.str(2), nullptr, 10);
            } catch (std::exception &e) {
                this->dim = 0;
            }
            this->x = std::stoi(match.str(4), nullptr, 10);
            this->z = std::stoi(match.str(5), nullptr, 10);
        }

        lce::io::BinaryIO io(data.data());

        std::vector<ChunkLocation> locations(1024);
        std::vector<uint32_t> timestamps(1024);
        std::vector<Chunk> rChunks;

        for (short i = 0; i < 1024; i++) {
            ChunkLocation loc{};

            if (byteOrder == io::ByteOrder::LITTLE)
                loc.size = io.readByte() * 4096;

            loc.offset = io.readInt24(byteOrder) * 4096;

            if (byteOrder == io::ByteOrder::BIG)
                loc.size = io.readByte() * 4096;

            // DebugLog("Chunk at " << loc.offset << ": " << loc.size);

            locations[i] = loc;
        }

        for (short i = 0; i < 1024; i++) {
            timestamps[i] = io.read<int32_t>(byteOrder);
        }

        for (short c = 0; c < 1024; c++) {
            RegionChunk chunk;

            chunk.location = locations[c];
            chunk.timestamp = timestamps[c];

            std::vector<uint8_t> chunkData(chunk.location.size);

            chunk.chunk = nullptr;

            if (chunk.location.size != 0) {
                io.seek(chunk.location.offset);
                // std::cout << std::hex << io.getPosition() << std::endl;
                io.readInto(chunkData.data(), chunk.location.size);

                chunk.chunk = new Chunk(chunkData, outerCompression, byteOrder);
            }

            chunks[c] = chunk;
        }
    }

    std::map<int16_t, int16_t>
    Region::getXZFromFilename(const std::wstring &name) {
        std::map<int16_t, int16_t> result;
        const std::wregex re(
            L"(DIM([-0-9]{1,2}))?(r)\\.([-0-9]{1,2})\\.([-0-9]{1,2}).mcr");

        std::wsmatch match;

        if (std::regex_match(name, match, re)) {
            int x = std::stoi(match.str(4), nullptr, 10);
            int z = std::stoi(match.str(5), nullptr, 10);

            result.insert({1, x});
            result.insert({2, z});
        }

        return result;
    }

    int16_t Region::getDimFromFilename(std::wstring name) {
        const std::wregex re(
            L"(DIM([-0-9]{1,2}))?(r)\\.([-0-9]{1,2})\\.([-0-9]{1,2}).mcr");
        std::wsmatch match;

        if (std::regex_match(name, match, re)) {
            const int dim = std::stoi(match.str(2), nullptr, 10);
            return dim;
        }

        return -9999;
    }

    int16_t Region::getX() const { return this->x; }

    int16_t Region::getZ() const { return this->z; }

    int16_t Region::getDim() const { return this->dim; }

    void Region::setX(int16_t x) { this->x = x; }

    void Region::setZ(int16_t z) { this->z = z; }

    void Region::setDim(int16_t dim) { this->dim = dim; }
} // namespace lce::world
