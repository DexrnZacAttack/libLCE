#include "library.h"

#include <chrono>
#include <fstream>
#include <iostream>

#include "src/arc/Archive.h"
#include "src/save/SaveFileOld.h"
#include "src/save/SaveFile.h"

namespace tests {
    void saveTest() {
        FILE* f = fopen("savegame.dat", "rb");
        fseek(f, 0, SEEK_END);
        const size_t endPos = ftell(f);
        fseek(f, 0, SEEK_SET);
        const size_t beginPos = ftell(f);
        const size_t length = endPos - beginPos;

        uint8_t* ass = new uint8_t[length];

        fread(ass, 1, length, f);

        fclose(f);

        lce::save::SaveFile file = lce::save::SaveFile::readFromData(ass);
        std::cout << "saveTest: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.version) << std::endl;
    }

    void arcTest() {
        FILE* f = fopen("example.arc", "rb");
        fseek(f, 0, SEEK_END);
        const size_t endPos = ftell(f);
        fseek(f, 0, SEEK_SET);
        const size_t beginPos = ftell(f);
        const size_t length = endPos - beginPos;

        uint8_t* ass = new uint8_t[length];

        fread(ass, 1, length, f);

        fclose(f);

        lce::arc::Archive::readFromData(ass);
    }

    void oldSaveTest() {
        FILE* f = fopen("savegame_pr.dat", "rb");
        fseek(f, 0, SEEK_END);
        const size_t endPos = ftell(f);
        fseek(f, 0, SEEK_SET);
        const size_t beginPos = ftell(f);
        const size_t length = endPos - beginPos;

        uint8_t* ass = new uint8_t[length];

        fread(ass, 1, length, f);

        fclose(f);

        lce::save::SaveFileOld file = lce::save::SaveFileOld::readFromData(ass);
        std::cout << "oldSaveTest: " << "File version is " << static_cast<lce::save::SaveFileVersion>(file.version) << std::endl;

        const uint8_t* file2 = file.create();

        std::ofstream outFile("testsavegame.dat", std::ios::binary);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file");
        }

        outFile.write(reinterpret_cast<const char*>(file2), file.getSize());
        if (!outFile) {
            throw std::ios_base::failure("Failed to write");
        }

        outFile.close();
    }


    void runTest(void (*test)(), const std::string name) {
        const auto startTime = std::chrono::high_resolution_clock::now();

        std::cout << "Running test \"" << name << "\"" << std::endl;
        test();

        const auto endTime = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "\"" << name << "\" finished after " << duration.count() << "ms." << std::endl;
    }

    int run() {
        runTest(saveTest, "Read savegame.dat");
        runTest(oldSaveTest, "Read PR savegame.dat");
        runTest(arcTest, "Read example.arc");
        return 0;
    }
}

int main(int argc, char** argv) {
    tests::run();

    return 0;
}
