#include <filesystem>

#include <libLCEExports.h>

#include "tests/formats.h"
#include "tests/vfs.h"
#include <util.h>

int main(int argc, char **argv) {
    lce::printLibraryInfo();

    std::filesystem::create_directories(lce::tests::util::examples); // input
    std::filesystem::create_directories(lce::tests::util::output);   // output

    ADD_TESTS(RUN_FORMATS_TESTS, lce::tests::formats::run);
    ADD_TESTS(RUN_VFS_TESTS, lce::tests::vfs::run);

    return 0;
};