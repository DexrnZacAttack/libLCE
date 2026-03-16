//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#include "formats.h"
#include "LCE/archive/Archive.h"
#include "LCE/color/ColorFile.h"
#include "LCE/compression/Compression.h"
#include "LCE/localization/LocalizationFile.h"
#include "LCE/save/SaveFile.h"
#include "LCE/save/Thumb.h"
#include "LCE/soundbank/Soundbank.h"
#include "util.h"
#include "BinaryIO/stream/BinaryInputStream.h"
#include "BinaryIO/stream/BinaryOutputStream.h"

#include <BinaryIO/buffer/BinaryBuffer.h>

namespace lce::tests::formats {
    void arcTest() {
        std::ifstream in(util::examples / "example.arc", std::ifstream::binary);

        bio::stream::BinaryInputStream b = bio::stream::BinaryInputStream(in);
        std::unique_ptr<arc::Archive> file = b.deserialize<arc::Archive::Deserializer>();

        std::ofstream out(util::output / "output.arc", std::ios::binary);

        bio::stream::BinaryOutputStream ob(out);
        ob.serialize<arc::Archive::Serializer>(*file.get());
    }

    void locTest() {
        std::string n = "example.loc";

        OPEN_FILE(n, f);

        loc::LocalizationFile file = loc::LocalizationFile(f.data());

#if WRITE_LOC
        std::filesystem::create_directories(util::output / (n + ".dir"));
        for (auto &[i, l] : file.getLanguages()) {
            std::ofstream o(util::output / (n + ".dir") /
                            (i.getName() + ".txt"));

            for (const auto &[id, str] : l.getStrings()) {
                o << "[0x" << std::hex << id << "] " << str << std::endl;
            }

            o.close();
        }
#endif

        file.setString("en-EN", "test.hello", "Hello!");
        file.setString("fi-FI", "test.hello", "Hei!");

        if (!file.languageExists("TestLang")) {
            file.createLanguage("TestLang");
        }

        file.setString("TestLang", "test.hello", "Hello, world!");

        std::cout << file.getString("en-EN", "test.hello") << std::endl;

        WRITE_FILE("output.loc", reinterpret_cast<char *>(file.serialize()),
                   file.getSize());
    }

    void msscmpTest(bio::util::ByteOrder endian) {
        std::string order = endian == bio::util::ByteOrder::LITTLE ? "le" : "be";
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
        // save::SaveFileOld file = save::SaveFileOld(f, bio::util::ByteOrder::BIG);

        // DebugLog("oldSaveTest: File version is " << file->getVersion());

#ifdef CMAKE_BUILD_DEBUG
        // for (const auto &[name, child] : file->getRoot()->getChildren()) {
        //     DebugLogW(name);
        // }
#endif

#if WRITE_FS
        file.getRoot()->writeOut(util::output / "savegame_pr");
#endif

        // WRITE_FILE("savegame_pr-be_out.dat",
        //            reinterpret_cast<char *>(file.serialize()), file.getSize());
        //
        // // write le file
        // file.setEndian(bio::util::ByteOrder::LITTLE);
        //
        // _WRITE_FILE("savegame_pr_switch-to-le_out.dat",
        //             reinterpret_cast<char *>(file.serialize()), file.getSize(),
        //             outSwitch);
    }

    void saveFromFolderTest() {
        std::filesystem::create_directories(util::examples / "FSSaveFolder");
        fs::Directory *d = new fs::Directory(util::examples / "FSSaveFolder");
        fs::Filesystem *fs = new fs::Filesystem(d);

        save::SaveFile file = save::SaveFile(*fs);

#if WRITE_FS
        file.getRoot()->writeOut(util::output / "savegame-from-folder");
#endif

        // _WRITE_FILE("savegame-from-folder.dat",
        //             reinterpret_cast<char *>(file.serialize()), file.getSize(),
        //             outFolder);

        // save::SaveFileOld *old =
        //     dynamic_cast<save::SaveFileOld *>(file.migrateVersion(1));
        //
        // _WRITE_FILE("savegame-from-folder-old.dat",
        //             reinterpret_cast<char *>(old->serialize()), old->getSize(),
        //             outOld);
    }

    void saveTestEndian(bio::util::ByteOrder endian) {
        const std::string order =
            endian == bio::util::ByteOrder::LITTLE ? "le" : "be";
        const std::string inName = "savegame-" + order + ".dat";
        const std::string outName = "savegame-" + order + "_out.dat";

        std::ifstream in(util::examples / inName, std::ifstream::binary);

        bio::stream::BinaryInputStream b = bio::stream::BinaryInputStream(in);
        std::unique_ptr<save::SaveFile> file = b.deserialize<save::SaveFile::Deserializer>(save::SaveFile::SerializerOptions {
            endian
        });

#ifdef CMAKE_BUILD_DEBUG
        for (const auto &[name, child] : file->getRoot()->getChildren()) {
            DebugLogW(name);
        }
#endif

#if WRITE_FS
        file.getRoot()->writeOut(util::output / ("savegame-" + order));
#endif

        in.close();

        {
            std::ofstream out(util::output / outName, std::ofstream::binary);

            bio::stream::BinaryOutputStream ob(out);
            ob.serialize<save::SaveFile::Serializer>(*file.get(), save::SaveFile::SerializerOptions {
                endian
            });
        }

        std::ifstream validate(util::output / outName, std::ifstream::binary);

        bio::stream::BinaryInputStream bis = bio::stream::BinaryInputStream(validate);
        std::unique_ptr<save::SaveFile> validateFile = bis.deserialize<save::SaveFile::Deserializer>(save::SaveFile::SerializerOptions {
            endian
        });
    }

    void saveTestVita() {
        OPEN_FILE("savegame-vita.dat", f);
        std::vector<uint8_t> fd;

        const uint64_t s = compression::Compression::getCompressedSaveFileSize(
            f, bio::util::ByteOrder::LITTLE);

        if (bool dc = compression::Compression::decompressVita(f, fd, s, 8);
            dc == false)
            throw std::ios_base::failure("Failed to decompress Vita");

        WRITE_FILE("savegame-vita_dc.dat", reinterpret_cast<char *>(fd.data()),
                   fd.size());

        // save::SaveFile file = save::SaveFile(fd, bio::util::ByteOrder::LITTLE);

#if WRITE_FS
        file.getRoot()->writeOut(util::output / "savegame-vita");
#endif

        // _WRITE_FILE("savegame-vita_out.dat",
        //             reinterpret_cast<char *>(file.serialize()), file.getSize(),
        //             outVita);
    }

    // void regionTest() {
    //     OPEN_FILE("regions/r.0.0.mcr", f);
    //
    //     const world::Region file =
    //         world::Region(f, L"r.0.0.mcr",
    //         compression::Compression::Type::ZLIB,
    //                       bio::util::ByteOrder::BIG);
    //     std::cout << "regionTest: " << "X: " << file.getX()
    //               << ", Z: " << file.getZ() << ", DIM: " << file.getDim()
    //               << std::endl;
    // }

    void saveTestSwitch(bio::util::ByteOrder endian) {
        std::string order = endian == bio::util::ByteOrder::LITTLE ? "le" : "be";
        std::string rOrder = endian == bio::util::ByteOrder::LITTLE ? "be" : "le";
        std::string inName = "savegame-" + rOrder + ".dat";
        std::string outName =
            "savegame-" + rOrder + "_switch-to-" + order + "_out.dat";

        OPEN_FILE(inName, f);

        // save::SaveFile file = save::SaveFile(f, endian == bio::util::ByteOrder::LITTLE
        //                                             ? bio::util::ByteOrder::BIG
        //                                             : bio::util::ByteOrder::LITTLE);

#if WRITE_FS
        file.getRoot()->writeOut(util::output / ("savegame-endian_switch-to-" +
                                                 rOrder + "_orig-" + order));
#endif

        // file.setEndian(endian);

        // WRITE_FILE(outName, reinterpret_cast<char *>(file.serialize()),
        //            file.getSize());
    }

    void colorWriteTest(const color::ColorFileCommons &colors) {
        // WRITE_FILE("output.col",
        //            reinterpret_cast<const char *>(colors.serialize()),
        //            colors.getSize());
    }

    void colorTest() {
        OPEN_FILE("colours.col", f);

        std::cout << "Read" << std::endl;

        colorWriteTest(*color::ColorFileCommons::deserializeAuto(f));
    }

    void thumbTest(const bio::util::ByteOrder endian, int headerSize, bool use4Byte) {
        const std::string order =
            endian == bio::util::ByteOrder::LITTLE ? "le" : "be";
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