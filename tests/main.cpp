#include <chrono>
#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

#include <Archive/Archive.h>
#include <Color/ColorFile.h>
#include <Save/SaveFileOld.h>
#include <Save/Thumb.h>
#include <World/Region.h>
#include <libLCEExports.h>

#include <Localization/LocalizationFile.h>
#include <Soundbank/Soundbank.h>

#include "util.h"

#define _OPEN_FILE(path, out, name) \
    std::ifstream name(util::examples / path, std::ifstream::binary); \
    \
    if (!name.is_open()) throw std::ios_base::failure(std::string("Failed to open file ") + (util::examples / path).string()); \
    \
    std::vector<uint8_t> out(std::filesystem::file_size(util::examples / path)); \
    name.read(reinterpret_cast<char *>(out.data()), out.size())

#define _WRITE_FILE(path, data, size, name) \
    std::ofstream name(util::output / path, std::ios::binary); \
    \
    if (!name) throw std::ios_base::failure("Failed to open file"); \
    \
    name.write(data, size); \
    \
    if (!name) throw std::ios_base::failure("Failed to write"); \
    \
    name.close()

#define OPEN_FILE(path, out) _OPEN_FILE(path, out, in)
#define WRITE_FILE(path, data, size) _WRITE_FILE(path, data, size, out)

namespace lce::tests {
    void arcTest() {
        OPEN_FILE("example.arc", f);

        arc::Archive file = arc::Archive(f.data());

#ifdef WRITE_FS
        file.getRoot()->writeOut(util::output / "arc");
#endif

        WRITE_FILE("output.arc", reinterpret_cast<char*>(file.toData()), file.getSize());
    }

    void locTest() {
        OPEN_FILE("example.loc", f);

        loc::LocalizationFile file = loc::LocalizationFile(f.data());

        WRITE_FILE("output.loc", reinterpret_cast<char*>(file.toData()), file.getSize());
    }

    void msscmpTest(io::ByteOrder endian) {
        std::string order = endian == io::ByteOrder::LITTLE ? "le" : "be";
        std::string name = "msscmp-" + order + ".msscmp";

        OPEN_FILE(name, f);

        msscmp::Soundbank file = msscmp::Soundbank(f.data());

#ifdef WRITE_FS
        file.getRoot()->writeOut(util::output / "msscmp");
#endif
    }

    void oldSaveTest() {
        OPEN_FILE("savegame_pr.dat", f);

        // read be file
        save::SaveFileOld file = save::SaveFileOld(f, io::ByteOrder::BIG);

        DebugLog("oldSaveTest: File version is " << file.getVersion());

#ifdef CMAKE_BUILD_DEBUG
        for (const auto& [name, child] : file.getRoot()->getChildren()) {
            DebugLogW(name);
        }
#endif

#ifdef WRITE_FS
        file.getRoot()->writeOut(util::output / "savegame_pr");
#endif

        WRITE_FILE("savegame_pr-be_out.dat", reinterpret_cast<char*>(file.toData()), file.getSize());

        // write le file
        file.setEndian(io::ByteOrder::LITTLE);

        _WRITE_FILE("savegame_pr_switch-to-le_out.dat", reinterpret_cast<char*>(file.toData()), file.getSize(), outSwitch);
    }

    void saveTestEndian(io::ByteOrder endian) {
        const std::string order = endian == io::ByteOrder::LITTLE ? "le" : "be";
        const std::string inName = "savegame-" + order + ".dat";
        const std::string outName = "savegame-" + order + "_out.dat";

        OPEN_FILE(inName, f);

        const save::SaveFile file = save::SaveFile(f, endian);
        // lce::tests::writeFS(&file, L"savegame-" + io::BinaryIO::stringToWString(order));

        DebugLog("saveTestEndian: File version is " << file.getVersion());

#ifdef CMAKE_BUILD_DEBUG
        for (const auto& [name, child] : file.getRoot()->getChildren()) {
            DebugLogW(name);
        }
#endif

#ifdef WRITE_FS
        file.getRoot()->writeOut(util::output / ("savegame-" + order));
#endif

        WRITE_FILE(outName, reinterpret_cast<char*>(file.toData()), file.getSize());
    }

    void saveTestVita() {
        OPEN_FILE("savegame-vita.dat", f);
        std::vector<uint8_t> fd;

        const uint64_t s = compression::Compression::getSizeFromSave(f, io::ByteOrder::LITTLE);

        if (bool dc = compression::Compression::decompressVita(f, fd, s, 8); dc == false)
            throw std::ios_base::failure("Failed to decompress Vita");

        WRITE_FILE("savegame-vita_dc.dat", reinterpret_cast<char*>(fd.data()), fd.size());

        save::SaveFile file = save::SaveFile(fd, io::ByteOrder::LITTLE);

#ifdef WRITE_FS
        file.getRoot()->writeOut(util::output / "savegame-vita");
#endif

        _WRITE_FILE("savegame-vita_out.dat", reinterpret_cast<char*>(file.toData()), file.getSize(), outVita);
    }

    void regionTest() {
        OPEN_FILE("regions/r.0.0.mcr", f);

        const world::Region file =
            world::Region(f, L"r.0.0.mcr", compression::CompressionType::ZLIB, io::ByteOrder::BIG);
        std::cout << "regionTest: " << "X: " << file.getX() << ", Z: " << file.getZ() << ", DIM: " << file.getDim()
                  << std::endl;
    }

    void saveTestSwitch(io::ByteOrder endian) {
        std::string order = endian == io::ByteOrder::LITTLE ? "le" : "be";
        std::string rOrder = endian == io::ByteOrder::LITTLE ? "be" : "le";
        std::string inName = "savegame-" + rOrder + ".dat";
        std::string outName = "savegame-" + rOrder + "_switch-to-" + order + "_out.dat";

        OPEN_FILE(inName, f);

        save::SaveFile file =
            save::SaveFile(f, endian == io::ByteOrder::LITTLE ? io::ByteOrder::BIG : io::ByteOrder::LITTLE);

#ifdef WRITE_FS
        file.getRoot()->writeOut(util::output / ("savegame-endian_switch-to-" + rOrder + "_orig-" + order));
#endif

        file.setEndian(endian);

        WRITE_FILE(outName, reinterpret_cast<char*>(file.toData()), file.getSize());
    }

    void colorWriteTest(const color::ColorFile& colors) {
        WRITE_FILE("output.col", reinterpret_cast<const char*>(colors.create()), colors.getSize());
    }

    void colorTest() {
        OPEN_FILE("colours.col", f);

        std::cout << "Read" << std::endl;

        colorWriteTest(color::ColorFile::read(f));
    }

    void thumbTest(const io::ByteOrder endian, int headerSize, bool use4Byte = false) {
        const std::string order = endian == io::ByteOrder::LITTLE ? "le" : "be";
        const std::string name = "THUMB-" + order + (use4Byte ? "_switch" : "");

        OPEN_FILE(name, f);

        auto file = save::Thumb(f, endian, headerSize, use4Byte);
        DebugLogW(file.getWorldName());
    }

    void compressedChunkTest() {
        DebugLog("Reading");

        // TODO: shouldn't we be decompressing the original zlib chunk?

        OPEN_FILE("rle_chunk.dat", f);

        DebugLog("Decompressing");

        std::vector<uint8_t> dc(0x40000);
        lce::compression::Compression::decompressChunk(f, dc);

        DebugLog("Writing");

        WRITE_FILE("decompressed_chunk.dat", reinterpret_cast<char*>(dc.data()), dc.size());
    }
} // namespace lce::tests

#define ADD_TEST(testName, func)                                                                                       \
    if (testName)                                                                                                      \
        func;

int main(int argc, char** argv) {
    printLibraryInfo();

    try {
        std::filesystem::create_directories(lce::tests::util::examples); // input
        std::filesystem::create_directories(lce::tests::util::output); // output
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Couldn't create i/o dirs: " << e.what() << std::endl;
    }

    ADD_TEST(BE_SAVEGAME, lce::tests::util::runTest(lce::tests::saveTestEndian, "Read & Write Big Endian savegame.dat",
                                              lce::io::ByteOrder::BIG))
    ADD_TEST(LE_SAVEGAME, lce::tests::util::runTest(lce::tests::saveTestEndian, "Read & Write Little Endian savegame.dat",
                                              lce::io::ByteOrder::LITTLE))
    ADD_TEST(BE_TO_LE_SAVEGAME,
             lce::tests::util::runTest(lce::tests::saveTestSwitch, "Switch Big Endian to Little Endian savegame.dat",
                                 lce::io::ByteOrder::LITTLE))
    ADD_TEST(LE_TO_BE_SAVEGAME,
             lce::tests::util::runTest(lce::tests::saveTestSwitch, "Switch Little Endian to Big Endian savegame.dat",
                                 lce::io::ByteOrder::BIG))
    ADD_TEST(PRERELEASE_SAVEGAME, lce::tests::util::runTest(lce::tests::oldSaveTest, "Read & Write PR savegame.dat"))
    ADD_TEST(PSVITA_SAVEGAME, lce::tests::util::runTest(lce::tests::saveTestVita, "Read & Write PSVita savegame.dat"))
    ADD_TEST(ARC, lce::tests::util::runTest(lce::tests::arcTest, "Read example.arc"))
    ADD_TEST(LOC, lce::tests::util::runTest(lce::tests::locTest, "Read example.loc"))
    ADD_TEST(COL, lce::tests::util::runTest(lce::tests::colorTest, "Read COL file"))
    ADD_TEST(BE_THUMB,
             lce::tests::util::runTest(lce::tests::thumbTest, "Read Big Endian THUMB", lce::io::ByteOrder::BIG, 0x100, false))
    ADD_TEST(LE_THUMB, lce::tests::util::runTest(lce::tests::thumbTest, "Read Little Endian THUMB",
                                           lce::io::ByteOrder::LITTLE, 0x100, false))
    ADD_TEST(SWITCH_4B_WCHAR_THUMB,
             lce::tests::util::runTest(lce::tests::thumbTest, "Read Switch THUMB", lce::io::ByteOrder::LITTLE, 0x208, true))
    ADD_TEST(READ_COMPRESSED_CHUNK, lce::tests::util::runTest(lce::tests::compressedChunkTest, "Read compressed chunk"))
    ADD_TEST(READ_REGION, lce::tests::util::runTest(lce::tests::regionTest, "Read region"))
    ADD_TEST(NEWGEN_MSSCMP,
             lce::tests::util::runTest(lce::tests::msscmpTest, "Read newgen MSSCMP", lce::io::ByteOrder::LITTLE))
    ADD_TEST(OLDGENMSSCMP, lce::tests::util::runTest(lce::tests::msscmpTest, "Read oldgen MSSCMP", lce::io::ByteOrder::BIG))

    return 0;
}
