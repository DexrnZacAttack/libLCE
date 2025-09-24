//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#include "formats.h"
#include "Archive/Archive.h"
#include "Color/ColorFile.h"
#include "Localization/LocalizationFile.h"
#include "Save/SaveFile.h"
#include "Save/SaveFileOld.h"
#include "Save/Thumb.h"
#include "Soundbank/Soundbank.h"
#include "World/Region.h"
#include "util.h"

namespace lce::tests::formats {
    void arcTest() {
        OPEN_FILE("example.arc", f);

        arc::Archive file = arc::Archive(f.data());

#if WRITE_FS
        file.getRoot()->writeOut(util::output / "arc");
#endif

        WRITE_FILE("output.arc", reinterpret_cast<char *>(file.serialize()),
                   file.getSize());
    }

    void locTest() {
        OPEN_FILE("example.loc", f);

        loc::LocalizationFile file = loc::LocalizationFile(f.data());

        WRITE_FILE("output.loc", reinterpret_cast<char *>(file.serialize()),
                   file.getSize());
    }

    void msscmpTest(io::ByteOrder endian) {
        std::string order = endian == io::ByteOrder::LITTLE ? "le" : "be";
        std::string name = "msscmp-" + order + ".msscmp";

        OPEN_FILE(name, f);

        msscmp::Soundbank file = msscmp::Soundbank(f.data());

#if WRITE_FS
        file.getRoot()->writeOut(util::output / "msscmp");
#endif
    }

    void oldSaveTest() {
        OPEN_FILE("savegame_pr.dat", f);

        // read be file
        save::SaveFileOld file = save::SaveFileOld(f, io::ByteOrder::BIG);

        DebugLog("oldSaveTest: File version is " << file.getVersion());

#ifdef CMAKE_BUILD_DEBUG
        for (const auto &[name, child] : file.getRoot()->getChildren()) {
            DebugLogW(name);
        }
#endif

#if WRITE_FS
        file.getRoot()->writeOut(util::output / "savegame_pr");
#endif

        WRITE_FILE("savegame_pr-be_out.dat",
                   reinterpret_cast<char *>(file.serialize()), file.getSize());

        // write le file
        file.setEndian(io::ByteOrder::LITTLE);

        _WRITE_FILE("savegame_pr_switch-to-le_out.dat",
                    reinterpret_cast<char *>(file.serialize()), file.getSize(),
                    outSwitch);
    }

    void saveFromFolderTest() {
        fs::Directory *d = new fs::Directory(util::examples / "FSSaveFolder");
        fs::Filesystem *fs = new fs::Filesystem(d);

        save::SaveFile file = save::SaveFile(*fs);

#if WRITE_FS
        file.getRoot()->writeOut(util::output / "savegame-from-folder");
#endif

        _WRITE_FILE("savegame-from-folder.dat",
                    reinterpret_cast<char *>(file.serialize()), file.getSize(),
                    outFolder);

        save::SaveFileOld *old =
            dynamic_cast<save::SaveFileOld *>(file.migrateVersion(1));

        _WRITE_FILE("savegame-from-folder-old.dat",
                    reinterpret_cast<char *>(old->serialize()), old->getSize(),
                    outOld);
    }

    void saveTestEndian(io::ByteOrder endian) {
        const std::string order = endian == io::ByteOrder::LITTLE ? "le" : "be";
        const std::string inName = "savegame-" + order + ".dat";
        const std::string outName = "savegame-" + order + "_out.dat";

        OPEN_FILE(inName, f);

        const save::SaveFile file = save::SaveFile(f, endian);
        // lce::tests::writeFS(&file, L"savegame-" +
        // io::BinaryIO::stringToWString(order));

        DebugLog("saveTestEndian: File version is " << file.getVersion());

#ifdef CMAKE_BUILD_DEBUG
        for (const auto &[name, child] : file.getRoot()->getChildren()) {
            DebugLogW(name);
        }
#endif

#if WRITE_FS
        file.getRoot()->writeOut(util::output / ("savegame-" + order));
#endif

        WRITE_FILE(outName, reinterpret_cast<char *>(file.serialize()),
                   file.getSize());
    }

    void saveTestVita() {
        OPEN_FILE("savegame-vita.dat", f);
        std::vector<uint8_t> fd;

        const uint64_t s = compression::Compression::getCompressedSaveFileSize(
            f, io::ByteOrder::LITTLE);

        if (bool dc = compression::Compression::decompressVita(f, fd, s, 8);
            dc == false)
            throw std::ios_base::failure("Failed to decompress Vita");

        WRITE_FILE("savegame-vita_dc.dat", reinterpret_cast<char *>(fd.data()),
                   fd.size());

        save::SaveFile file = save::SaveFile(fd, io::ByteOrder::LITTLE);

#if WRITE_FS
        file.getRoot()->writeOut(util::output / "savegame-vita");
#endif

        _WRITE_FILE("savegame-vita_out.dat",
                    reinterpret_cast<char *>(file.serialize()), file.getSize(),
                    outVita);
    }

    void regionTest() {
        OPEN_FILE("regions/r.0.0.mcr", f);

        const world::Region file =
            world::Region(f, L"r.0.0.mcr", compression::Compression::Type::ZLIB,
                          io::ByteOrder::BIG);
        std::cout << "regionTest: " << "X: " << file.getX()
                  << ", Z: " << file.getZ() << ", DIM: " << file.getDim()
                  << std::endl;
    }

    void saveTestSwitch(io::ByteOrder endian) {
        std::string order = endian == io::ByteOrder::LITTLE ? "le" : "be";
        std::string rOrder = endian == io::ByteOrder::LITTLE ? "be" : "le";
        std::string inName = "savegame-" + rOrder + ".dat";
        std::string outName =
            "savegame-" + rOrder + "_switch-to-" + order + "_out.dat";

        OPEN_FILE(inName, f);

        save::SaveFile file = save::SaveFile(f, endian == io::ByteOrder::LITTLE
                                                    ? io::ByteOrder::BIG
                                                    : io::ByteOrder::LITTLE);

#if WRITE_FS
        file.getRoot()->writeOut(util::output / ("savegame-endian_switch-to-" +
                                                 rOrder + "_orig-" + order));
#endif

        file.setEndian(endian);

        WRITE_FILE(outName, reinterpret_cast<char *>(file.serialize()),
                   file.getSize());
    }

    void colorWriteTest(const color::ColorFileCommons &colors) {
        WRITE_FILE("output.col",
                   reinterpret_cast<const char *>(colors.serialize()),
                   colors.getSize());
    }

    void colorTest() {
        OPEN_FILE("colours.col", f);

        std::cout << "Read" << std::endl;

        colorWriteTest(*color::ColorFileCommons::deserializeAuto(f));
    }

    void thumbTest(const io::ByteOrder endian, int headerSize, bool use4Byte) {
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

        WRITE_FILE("decompressed_chunk.dat",
                   reinterpret_cast<char *>(dc.data()), dc.size());
    }
} // namespace lce::tests::formats