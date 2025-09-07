//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#pragma once
#include "util.h"

namespace lce::tests::vfs {
    void runForEachRecursive();
    void runForEachFilesOnlyRecursive();
    void runForEachDirectoriesOnlyRecursive();

    void runForEach();
    void runForEachFilesOnly();
    void runForEachDirectoriesOnly();

    void createFileFromReal();

    inline void run() {
        ADD_TEST(FOREACH_ALL, lce::tests::vfs::runForEach, util::Types::VFS,
                 "Foreach (All)");
        ADD_TEST(FOREACH_FILES, lce::tests::vfs::runForEachFilesOnly,
                 util::Types::VFS, "Foreach (Files)");
        ADD_TEST(FOREACH_DIRS, lce::tests::vfs::runForEachDirectoriesOnly,
                 util::Types::VFS, "Foreach (Directories)");

        ADD_TEST(FOREACH_ALL_RECURSE, lce::tests::vfs::runForEachRecursive,
                 util::Types::VFS, "Foreach (All, Recursive)");
        ADD_TEST(FOREACH_FILES_RECURSE,
                 lce::tests::vfs::runForEachFilesOnlyRecursive,
                 util::Types::VFS, "Foreach (Files, Recursive)");
        ADD_TEST(FOREACH_DIRS_RECURSE,
                 lce::tests::vfs::runForEachDirectoriesOnlyRecursive,
                 util::Types::VFS, "Foreach (Directories, Recursive)");

        ADD_TEST(FILE_FROM_REAL_FILE, lce::tests::vfs::createFileFromReal,
                 util::Types::VFS, "Create file from real FS file");
    }
} // namespace lce::tests::vfs
