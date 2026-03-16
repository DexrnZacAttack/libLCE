//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#pragma once
#include "LCE/color/ColorFile.h"
#include "util.h"
#include <TestFramework/test/ComparableTest.h>
#include <TestFramework/TestFramework.h>

#include <BinaryIO/buffer/BinaryBuffer.h>

namespace lce {
    namespace io {
        enum class ByteOrder;
    }

    namespace color {
        class ColorFile;
    }

    namespace tests::formats {
        // Saves
        void saveFromFolderTest(tfw::test::util::TestOutputLogger &logger);
        void saveTestEndian(tfw::test::util::TestOutputLogger &logger, bio::util::ByteOrder endian);
        void saveTestVita(tfw::test::util::TestOutputLogger &logger);
        void saveTestSwitch(tfw::test::util::TestOutputLogger &logger, bio::util::ByteOrder endian);
        void oldSaveTest(tfw::test::util::TestOutputLogger &logger);
        // World
        // void regionTest();
        void compressedChunkTest(tfw::test::util::TestOutputLogger &logger);
        // File formats
        void arcTest(tfw::test::util::TestOutputLogger &logger);
        void locTest(tfw::test::util::TestOutputLogger &logger);
        void msscmpTest(tfw::test::util::TestOutputLogger &logger, bio::util::ByteOrder endian);
        void colorWriteTest(tfw::test::util::TestOutputLogger &logger, const color::ColorFileCommons &colors);
        void colorTest(tfw::test::util::TestOutputLogger &logger);
        void thumbTest(tfw::test::util::TestOutputLogger &logger, bio::util::ByteOrder endian, int headerSize,
                       bool use4Byte = false);

        inline void run() {
            tfw::TestFramework *testFramework = tfw::TestFramework::getInstance();

            testFramework->addTest(SAVEGAME_FROM_FOLDER, "Create savegame from folder", lce::tests::formats::saveFromFolderTest);
            testFramework->addTest(BE_SAVEGAME, "Read & Write Big Endian savegame.dat", lce::tests::formats::saveTestEndian, bio::util::ByteOrder::BIG);
            testFramework->addTest(LE_SAVEGAME, "Read & Write Little Endian savegame.dat", lce::tests::formats::saveTestEndian, bio::util::ByteOrder::LITTLE);
            testFramework->addTest(BE_TO_LE_SAVEGAME, "Switch Big Endian to Little Endian savegame.dat", lce::tests::formats::saveTestSwitch, bio::util::ByteOrder::LITTLE);
            testFramework->addTest(LE_TO_BE_SAVEGAME, "Switch Little Endian to Big Endian savegame.dat", lce::tests::formats::saveTestSwitch, bio::util::ByteOrder::BIG);
            testFramework->addTest(PRERELEASE_SAVEGAME, "Read & Write PR savegame.dat", lce::tests::formats::oldSaveTest);
            testFramework->addTest(PSVITA_SAVEGAME, "Read & Write PSVita savegame.dat", lce::tests::formats::saveTestVita);
            testFramework->addTest(ARC, "Read example.arc", lce::tests::formats::arcTest);
            testFramework->addTest(LOC, "Read example.loc", lce::tests::formats::locTest);
            testFramework->addTest(COL, "Read COL file", lce::tests::formats::colorTest);
            testFramework->addTest(BE_THUMB, "Read Big Endian THUMB", lce::tests::formats::thumbTest, bio::util::ByteOrder::BIG, 0x100, false);
            testFramework->addTest(LE_THUMB, "Read Little Endian THUMB", lce::tests::formats::thumbTest, bio::util::ByteOrder::LITTLE, 0x100, false);
            testFramework->addTest(SWITCH_4B_WCHAR_THUMB, "Read Switch THUMB", lce::tests::formats::thumbTest, bio::util::ByteOrder::LITTLE, 0x208, true);
            testFramework->addTest(READ_COMPRESSED_CHUNK, "Read compressed chunk", lce::tests::formats::compressedChunkTest);
            testFramework->addTest(NEWGEN_MSSCMP, "Read newgen MSSCMP", lce::tests::formats::msscmpTest, bio::util::ByteOrder::LITTLE);
            testFramework->addTest(OLDGENMSSCMP, "Read oldgen MSSCMP", lce::tests::formats::msscmpTest, bio::util::ByteOrder::BIG);
        }
    } // namespace tests::formats
} // namespace lce
