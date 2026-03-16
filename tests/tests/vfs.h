//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#pragma once
#include "util.h"
#include <TestFramework/TestFramework.h>

namespace lce::tests::vfs {
    void runForEachRecursive(tfw::test::util::TestOutputLogger &logger);
    void runForEachFilesOnlyRecursive(tfw::test::util::TestOutputLogger &logger);
    void runForEachDirectoriesOnlyRecursive(tfw::test::util::TestOutputLogger &logger);

    void runForEach(tfw::test::util::TestOutputLogger &logger);
    void runForEachFilesOnly(tfw::test::util::TestOutputLogger &logger);
    void runForEachDirectoriesOnly(tfw::test::util::TestOutputLogger &logger);

    void createFileFromReal(tfw::test::util::TestOutputLogger &logger);

    inline void run() {
        tfw::TestFramework *testFramework = tfw::TestFramework::getInstance();


        testFramework->addTest(FOREACH_ALL, "Foreach (All)", lce::tests::vfs::runForEach);
        testFramework->addTest(FOREACH_FILES, "Foreach (Files)", lce::tests::vfs::runForEachFilesOnly);
        testFramework->addTest(FOREACH_DIRS, "Foreach (Directories)", lce::tests::vfs::runForEachDirectoriesOnly);

        testFramework->addTest(FOREACH_ALL_RECURSE, "Foreach (All, Recursive)", lce::tests::vfs::runForEachRecursive);
        testFramework->addTest(FOREACH_FILES_RECURSE, "Foreach (Files, Recursive)", lce::tests::vfs::runForEachFilesOnlyRecursive);
        testFramework->addTest(FOREACH_DIRS_RECURSE, "Foreach (Directories, Recursive)", lce::tests::vfs::runForEachDirectoriesOnlyRecursive);

        testFramework->addTest(FILE_FROM_REAL_FILE, "Create file from real FS file", lce::tests::vfs::createFileFromReal);
    }
} // namespace lce::tests::vfs
