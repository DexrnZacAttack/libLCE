//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#pragma once
#include "IO/BinaryIO.h"
#include "util.h"

namespace lce {
    namespace io {
        enum class ByteOrder;
    }

    namespace color {
        class ColorFile;
    }

    namespace tests::formats {
        // Saves
        void saveFromFolderTest();
        void saveTestEndian(io::ByteOrder endian);
        void saveTestVita();
        void saveTestSwitch(io::ByteOrder endian);
        void oldSaveTest();
        // World
        void regionTest();
        void compressedChunkTest();
        // File formats
        void arcTest();
        void locTest();
        void msscmpTest(io::ByteOrder endian);
        void colorWriteTest(const color::ColorFile &colors);
        void colorTest();
        void thumbTest(io::ByteOrder endian, int headerSize,
                       bool use4Byte = false);

        inline void run() {
            ADD_TEST(SAVEGAME_FROM_FOLDER,
                     lce::tests::formats::saveFromFolderTest,
                     util::Types::FORMATS, "Create savegame from folder");
            ADD_TEST(BE_SAVEGAME, lce::tests::formats::saveTestEndian,
                     util::Types::FORMATS,
                     "Read & Write Big Endian savegame.dat",
                     lce::io::ByteOrder::BIG);
            ADD_TEST(LE_SAVEGAME, lce::tests::formats::saveTestEndian,
                     util::Types::FORMATS,
                     "Read & Write Little Endian savegame.dat",
                     lce::io::ByteOrder::LITTLE);
            ADD_TEST(BE_TO_LE_SAVEGAME, lce::tests::formats::saveTestSwitch,
                     util::Types::FORMATS,
                     "Switch Big Endian to Little Endian savegame.dat",
                     lce::io::ByteOrder::LITTLE);
            ADD_TEST(LE_TO_BE_SAVEGAME, lce::tests::formats::saveTestSwitch,
                     util::Types::FORMATS,
                     "Switch Little Endian to Big Endian savegame.dat",
                     lce::io::ByteOrder::BIG);
            ADD_TEST(PRERELEASE_SAVEGAME, lce::tests::formats::oldSaveTest,
                     util::Types::FORMATS, "Read & Write PR savegame.dat");
            ADD_TEST(PSVITA_SAVEGAME, lce::tests::formats::saveTestVita,
                     util::Types::FORMATS, "Read & Write PSVita savegame.dat");
            ADD_TEST(ARC, lce::tests::formats::arcTest, util::Types::FORMATS,
                     "Read example.arc");
            ADD_TEST(LOC, lce::tests::formats::locTest, util::Types::FORMATS,
                     "Read example.loc");
            ADD_TEST(COL, lce::tests::formats::colorTest, util::Types::FORMATS,
                     "Read COL file");
            ADD_TEST(BE_THUMB, lce::tests::formats::thumbTest,
                     util::Types::FORMATS, "Read Big Endian THUMB",
                     lce::io::ByteOrder::BIG, 0x100, false);
            ADD_TEST(LE_THUMB, lce::tests::formats::thumbTest,
                     util::Types::FORMATS, "Read Little Endian THUMB",
                     lce::io::ByteOrder::LITTLE, 0x100, false);
            ADD_TEST(SWITCH_4B_WCHAR_THUMB, lce::tests::formats::thumbTest,
                     util::Types::FORMATS, "Read Switch THUMB",
                     lce::io::ByteOrder::LITTLE, 0x208, true);
            ADD_TEST(READ_COMPRESSED_CHUNK,
                     lce::tests::formats::compressedChunkTest,
                     util::Types::FORMATS, "Read compressed chunk");
            ADD_TEST(READ_REGION, lce::tests::formats::regionTest,
                     util::Types::FORMATS, "Read region");
            ADD_TEST(NEWGEN_MSSCMP, lce::tests::formats::msscmpTest,
                     util::Types::FORMATS, "Read newgen MSSCMP",
                     lce::io::ByteOrder::LITTLE);
            ADD_TEST(OLDGENMSSCMP, lce::tests::formats::msscmpTest,
                     util::Types::FORMATS, "Read oldgen MSSCMP",
                     lce::io::ByteOrder::BIG);
        }
    } // namespace tests::formats
} // namespace lce
