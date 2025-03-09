#include <chrono>
#include <codecvt>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdarg>

#include "src/arc/Archive.h"
#include "src/col/ColorFile.h"
#include "src/save/SaveFileOld.h"
#include "src/world/Chunk.h"

// #include "src/arc/Archive.h"
// #include "src/save/SaveFileOld.h"
// #include "src/save/SaveFile.h"

namespace lce::tests {
    void arcTest() {
        FILE* f = fopen("../testFiles/example.arc", "rb");
        fseek(f, 0, SEEK_END);
        const size_t endPos = ftell(f);
        fseek(f, 0, SEEK_SET);
        const size_t beginPos = ftell(f);
        const size_t length = endPos - beginPos;

        uint8_t* ass = new uint8_t[length];

        fread(ass, 1, length, f);

        fclose(f);

        lce::arc::Archive file = lce::arc::Archive::Archive(ass);

        const uint8_t* file2 = file.create();

        std::ofstream outFile("../testFiles/testarc.arc", std::ios::binary);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file");
        }

        outFile.write(reinterpret_cast<const char*>(file2), file.getSize());
        if (!outFile) {
            throw std::ios_base::failure("Failed to write");
        }

        outFile.close();
    }

    void oldSaveTest() {
        FILE* f = fopen("../testFiles/savegame_pr.dat", "rb");
        if (f == nullptr) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        fseek(f, 0, SEEK_END);
        const size_t endPos = ftell(f);
        fseek(f, 0, SEEK_SET);
        const size_t length = endPos;

        std::vector<uint8_t> ass(length);

        fread(ass.data(), 1, length, f);

        fclose(f);

        // read be file
        lce::save::SaveFileOld file = lce::save::SaveFileOld::SaveFileOld(ass, BIG);
        std::cout << "oldSaveTest: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.getVersion()) << std::endl;

        // write be file
        const uint8_t* file2 = file.create();

        std::ofstream outFile("../testFiles/savegame_pr-be_out.dat", std::ios::binary);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file");
        }

        outFile.write(reinterpret_cast<const char*>(file2), file.getSize());
        if (!outFile) {
            throw std::ios_base::failure("Failed to write");
        }

        outFile.close();

        // write le file
        file.setEndian(LITTLE);

        std::ofstream outFile2("../testFiles/savegame_pr_switch-to-le_out.dat", std::ios::binary);
        if (!outFile2) {
            throw std::ios_base::failure("Failed to open file");
        }

        outFile2.write(reinterpret_cast<const char*>(file2), file.getSize());
        if (!outFile2) {
            throw std::ios_base::failure("Failed to write");
        }

        outFile2.close();
    }

    void saveTestEndian(ByteOrder endian) {
        std::string order = endian == ByteOrder::LITTLE ? "le" : "be";

        std::ifstream f("../testFiles/savegame-" + order + ".dat", std::ios::in | std::ios::binary | std::ios::ate);
        if (!f.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        f.seekg( 0, std::ios::end );
        uint64_t size = f.tellg();
        f.seekg( 0, std::ios::beg );

        std::vector<uint8_t> ass(size);

        f.read(reinterpret_cast<char *>(ass.data()), size);

        lce::save::SaveFile file = lce::save::SaveFile::SaveFile(ass, endian);
        std::cout << "saveTestEndian: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.getVersion()) << std::endl;

        const uint8_t* file2 = file.create();

        std::ofstream outFile("../testFiles/savegame-" + order + "_out.dat", std::ios::binary);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file");
        }

        outFile.write(reinterpret_cast<const char*>(file2), file.getSize());
        if (!outFile) {
            throw std::ios_base::failure("Failed to write");
        }

        outFile.close();
    }

    void saveTestSwitch(ByteOrder endian) {
        std::string order = endian == ByteOrder::LITTLE ? "le" : "be";

        std::ifstream f("../testFiles/savegame-" + std::string(endian == LITTLE ? "be" : "le") + ".dat", std::ios::in | std::ios::binary | std::ios::ate);
        if (!f.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        f.seekg( 0, std::ios::end );
        uint64_t size = f.tellg();
        f.seekg( 0, std::ios::beg );

        std::vector<uint8_t> ass(size);

        f.read(reinterpret_cast<char *>(ass.data()), size);

        lce::save::SaveFile file = lce::save::SaveFile::SaveFile(ass, endian == LITTLE ? BIG : LITTLE);
        std::cout << "saveTestSwitch: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.getVersion()) << std::endl;

        file.setEndian(endian);
        const uint8_t* file2 = file.create();

        std::ofstream outFile("../testFiles/savegame-" + std::string(endian == LITTLE ? "be" : "le") + "_switch-to-" + order + "_out.dat", std::ios::binary);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file");
        }

        outFile.write(reinterpret_cast<const char*>(file2), file.getSize());
        if (!outFile) {
            throw std::ios_base::failure("Failed to write");
        }

        outFile.close();
    }

    void colorWriteTest(color::ColorFile colors) {
        std::ofstream outFile("../testFiles/out.col", std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to create the file." << std::endl;
        }

        outFile.write(reinterpret_cast<const char *>(colors.create()), colors.getSize());

        outFile.close();
    }

    void colorTest() {
        std::ifstream fin("../testFiles/colours.col", std::ios::binary);
        std::vector<uint8_t> v(std::istreambuf_iterator<char>(fin), {});

        fin.close();

        std::cout << "Read" << std::endl;

        lce::color::ColorFile file = lce::color::ColorFile::read(v);

        colorWriteTest(file);
    }

    void compressedChunkTest() {
        std::ifstream fin("chunk.dat");

        std::vector<uint8_t> v;

        uint8_t c;
        while (fin >> c) {
            v.push_back(c);
        }

        fin.close();

        lce::world::CompressedChunk file = lce::world::CompressedChunk::CompressedChunk(v, BIG);

        std::cout << "Write time!" << std::endl;
        std::ifstream find("../testFiles/rle_chunk.dat");

        std::vector<uint8_t> vd;

        uint8_t cd;
        while (find >> cd) {
            vd.push_back(cd);
        }

        find.close();

        std::vector<uint8_t> decompressed = file.decompressInner(vd);

        std::ofstream dc("outchunk.dat", std::ios::binary);
        if (!dc) {
            throw std::ios_base::failure("Failed to open file for writing");
        }

        dc.write(reinterpret_cast<const char*>(decompressed.data()), decompressed.size());
    }

    template<class... Args>
    void runTest(void (*test)(Args...), const std::string name, Args... args) {
        const auto startTime = std::chrono::high_resolution_clock::now();

        std::cout << "Running test \"" << name << "\"" << std::endl;
        test(args...);

        const auto endTime = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "\"" << name << "\" finished after " << duration.count() << "ms." << std::endl;
    }

    int run() {

        return 0;
    }
}

int main(int argc, char** argv) {
    std::cout << "Init" << std::endl;

    lce::tests::runTest(lce::tests::saveTestEndian, "Read Big Endian savegame.dat", ByteOrder::BIG);
    lce::tests::runTest(lce::tests::saveTestEndian, "Read Little Endian savegame.dat", ByteOrder::LITTLE);
    lce::tests::runTest(lce::tests::saveTestSwitch, "Switch Big Endian to Little Endian savegame.dat", ByteOrder::LITTLE);
    lce::tests::runTest(lce::tests::saveTestSwitch, "Switch Little Endian to Big Endian savegame.dat", ByteOrder::BIG);
    lce::tests::runTest(lce::tests::oldSaveTest, "Read PR savegame.dat");
    lce::tests::runTest(lce::tests::arcTest, "Read example.arc");
    lce::tests::runTest(lce::tests::colorTest, "Read COL file");
    // lce::tests::runTest(lce::tests::compressedChunkTest, "Read compressed chunk");
    return 0;
}