#include <chrono>
#include <codecvt>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdarg>

#include "src/FileTypes/Archive/Archive.h"
#include "src/FileTypes/Color/ColorFile.h"
#include "src/Save/SaveFileOld.h"
#include "src/Save/Thumb.h"
#include "src/World/Chunk.h"
#include "src/World/Region.h"
#include "src/libLCEExports.h"

#include "src/FileTypes/Localization/LocalizationFile.h"

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

        lce::arc::Archive file = lce::arc::Archive(ass);

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
    
    void locTest() {
        FILE* f = fopen("../testFiles/example.loc", "rb");
        if (f == nullptr) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        fseek(f, 0, SEEK_END);
        const size_t endPos = ftell(f);
        fseek(f, 0, SEEK_SET);
        const size_t beginPos = ftell(f);
        const size_t length = endPos - beginPos;

        uint8_t* ass = new uint8_t[length];

        fread(ass, 1, length, f);

        lce::loc::LocalizationFile file = lce::loc::LocalizationFile(ass);
		
		const uint8_t* file2 = file.create();

        std::ofstream outFile("../testFiles/example_copy.loc", std::ios::binary);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file");
        }

        outFile.write(reinterpret_cast<const char*>(file2), file.getSize());
        if (!outFile) {
            throw std::ios_base::failure("Failed to write");
        }

        outFile.close();
		
        fclose(f);
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
        lce::save::SaveFileOld file = lce::save::SaveFileOld(ass, BIG);
        std::cout << "oldSaveTest: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.getVersion()) << std::endl;

        for (save::IndexInnerFile f : file.getFiles()) {
            
            std::wcout << lce::io::BinaryIO::u16stringToWstring(f.getName().c_str()) << std::endl;

        }


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

        outFile2.write(reinterpret_cast<const char*>(file.create()), file.getSize());
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

        lce::save::SaveFile file = lce::save::SaveFile(ass, endian);
        std::cout << "saveTestEndian: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.getVersion()) << std::endl;

        for (save::IndexInnerFile f : file.getFiles()) {
            
            std::wcout << lce::io::BinaryIO::u16stringToWstring(f.getName()) << std::endl;

        }

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

    void saveTestVita() {

        std::ifstream f("../testFiles/savegame-vita.dat", std::ios::in | std::ios::binary | std::ios::ate);
        if (!f.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        f.seekg( 0, std::ios::end );
        uint64_t size = f.tellg();
        f.seekg( 0, std::ios::beg );

        std::vector<uint8_t> ass(size);
        std::vector<uint8_t> assd;

        f.read(reinterpret_cast<char *>(ass.data()), size);


        // this works due to Oversight:tm: because I always push_back.
        uint64_t s = lce::compression::Compression::getSizeFromSave(ass, LITTLE);
        std::cout << s << std::endl;
        bool dc = lce::compression::Compression::decompressVita(ass, assd, s, 8);

        if (dc == false) {
            std::cerr << "Failed to decompress vita" << std::endl;
        }

        std::ofstream outDFile("../testFiles/savegame-vita_dc.dat", std::ios::binary);
        if (!outDFile) {
            throw std::ios_base::failure("Failed to open file");
        }

        outDFile.write(reinterpret_cast<const char*>(assd.data()), assd.size());
        if (!outDFile) {
            throw std::ios_base::failure("Failed to write");
        }

        lce::save::SaveFile file = lce::save::SaveFile(assd, LITTLE);
        std::cout << "saveTestEndian: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.getVersion()) << std::endl;

        for (save::IndexInnerFile f : file.getFiles()) {
            
            std::wcout << lce::io::BinaryIO::u16stringToWstring(f.getName()) << std::endl;
        }

        const uint8_t* file2 = file.create();

        std::ofstream outFile("../testFiles/savegame-vita_out.dat", std::ios::binary);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file");
        }

        outFile.write(reinterpret_cast<const char*>(file2), file.getSize());
        if (!outFile) {
            throw std::ios_base::failure("Failed to write");
        }

        outFile.close();
    }

    void regionTest() {

        std::ifstream f("../testFiles/DIM-1r.-1.-1.mcr", std::ios::in | std::ios::binary | std::ios::ate);
        if (!f.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        f.seekg( 0, std::ios::end );
        uint64_t size = f.tellg();
        f.seekg( 0, std::ios::beg );

        std::vector<uint8_t> ass(size);

        f.read(reinterpret_cast<char *>(ass.data()), size);

        lce::world::Region file = lce::world::Region(ass, L"DIM-1r.-1.-1.mcr", lce::compression::CompressionType::ZLIB, BIG);
        std::cout << "regionTest: " << "X: " << file.getX() << ", Z: " <<  file.getZ() << ", DIM: " << file.getDim() << std::endl;

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

        lce::save::SaveFile file = lce::save::SaveFile(ass, endian == LITTLE ? BIG : LITTLE);
        std::cout << "saveTestSwitch: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.getVersion()) << std::endl;

        for (save::IndexInnerFile f : file.getFiles()) {
            
            std::wcout << lce::io::BinaryIO::u16stringToWstring(f.getName()) << std::endl;
        }


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

    void thumbTest(ByteOrder endian, int headerSize, bool use4Byte = false) {
        std::string order = endian == ByteOrder::LITTLE ? "le" : "be";

        std::ifstream f("../testFiles/THUMB-" + order + (use4Byte ? "_switch" : ""), std::ios::in | std::ios::binary | std::ios::ate);
        if (!f.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        f.seekg( 0, std::ios::end );
        uint64_t size = f.tellg();
        f.seekg( 0, std::ios::beg );

        std::vector<uint8_t> ass(size);

        f.read(reinterpret_cast<char *>(ass.data()), size);

        lce::save::Thumb file = lce::save::Thumb(ass, endian, headerSize, use4Byte);
        std::wstring name = file.getWorldName();
        std::wcout << name << std::endl;
    }

    void compressedChunkTest() {
        std::ifstream fin("chunk.dat");

        std::vector<uint8_t> v;

        uint8_t c;
        while (fin >> c) {
            v.push_back(c);
        }

        fin.close();

        std::cout << "Write time!" << std::endl;
        std::ifstream find("../testFiles/rle_chunk.dat");

        std::vector<uint8_t> vd;

        uint8_t cd;
        while (find >> cd) {
            vd.push_back(cd);
        }

        find.close();

        std::ofstream dc("outchunk.dat", std::ios::binary);
        if (!dc) {
            throw std::ios_base::failure("Failed to open file for writing");
        }
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
    printLibraryInfo();

    lce::tests::runTest(lce::tests::saveTestEndian, "Read Big Endian savegame.dat", ByteOrder::BIG);
    lce::tests::runTest(lce::tests::saveTestEndian, "Read Little Endian savegame.dat", ByteOrder::LITTLE);
    lce::tests::runTest(lce::tests::saveTestSwitch, "Switch Big Endian to Little Endian savegame.dat", ByteOrder::LITTLE);
    lce::tests::runTest(lce::tests::saveTestSwitch, "Switch Little Endian to Big Endian savegame.dat", ByteOrder::BIG);
    lce::tests::runTest(lce::tests::oldSaveTest, "Read PR savegame.dat");
    lce::tests::runTest(lce::tests::saveTestVita, "Read PSVita savegame.dat");
    // lce::tests::runTest(lce::tests::arcTest, "Read example.arc");
    lce::tests::runTest(lce::tests::locTest, "Read example.loc");
    // lce::tests::runTest(lce::tests::colorTest, "Read COL file");
    // lce::tests::runTest(lce::tests::thumbTest, "Read Big Endian THUMB", ByteOrder::BIG, 0x100, false);
    // lce::tests::runTest(lce::tests::thumbTest, "Read Little Endian THUMB", ByteOrder::LITTLE, 0x100, false);
    // lce::tests::runTest(lce::tests::thumbTest, "Read Switch THUMB", ByteOrder::LITTLE, 0x208, true);
    // lce::tests::runTest(lce::tests::compressedChunkTest, "Read compressed chunk");
    lce::tests::runTest(lce::tests::regionTest, "Read region");
    return 0;
}
