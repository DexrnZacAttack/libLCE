//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#include "vfs.h"

#include "TestFramework/TestFramework.h"
#include "LCE/filesystem/Directory.h"
#include "LCE/filesystem/Filesystem.h"

namespace lce::tests::vfs {
    void runForEachRecursive(tfw::test::util::TestOutputLogger &logger) {
        const std::filesystem::path p(util::examples / "VFSTestFolder");
        const fs::Directory *d = new fs::Directory(p);

        std::cout << "Reading " << p << std::endl;
        std::cout << "Found " << d->getDirectoryCount() + d->getFileCount()
                  << " children ";
        std::cout << "(" << d->getFileCount() << " files, ";
        std::cout << d->getDirectoryCount() << " directories)" << std::endl;

        d->forEachRecursive([](const fs::FSObject::name_t &name,
                               const fs::FSObject &f) {
            if (f.isFile()) {
                std::cout << dynamic_cast<const fs::File &>(f) << std::endl;
            } else {
                std::cout << dynamic_cast<const fs::Directory &>(f)
                           << std::endl;
            }
        });
    }

    void runForEachFilesOnlyRecursive(tfw::test::util::TestOutputLogger &logger) {
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEachFilesRecursive(
            [](const fs::FSObject::name_t &name, const fs::File &f) {
                std::cout << f << std::endl;
            });
    }

    void runForEachDirectoriesOnlyRecursive(tfw::test::util::TestOutputLogger &logger) {
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEachDirectoriesRecursive(
            [](const fs::FSObject::name_t &name, const fs::Directory &f) {
                std::cout << f << std::endl;
            });
    }

    void runForEach(tfw::test::util::TestOutputLogger &logger) {
        std::filesystem::create_directories(util::examples / "VFSTestFolder");
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEach([](const fs::FSObject::name_t &name, const fs::FSObject &f) {
            if (f.isFile()) {
                std::cout << dynamic_cast<const fs::File &>(f) << std::endl;
            } else {
                std::cout << dynamic_cast<const fs::Directory &>(f)
                           << std::endl;
            }
        });
    }

    void runForEachFilesOnly(tfw::test::util::TestOutputLogger &logger) {
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEachFiles([](const fs::FSObject::name_t &name, const fs::File &f) {
            std::cout << f << std::endl;
        });
    }

    void runForEachDirectoriesOnly(tfw::test::util::TestOutputLogger &logger) {
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEachDirectories(
            [](const fs::FSObject::name_t &name, const fs::Directory &f) {
                std::cout << f << std::endl;
            });
    }

    void createFileFromReal(tfw::test::util::TestOutputLogger &logger) {
        const fs::File file(util::examples / "VFSTestFile");

        std::cout << file << std::endl;

        file.writeOut(util::output / "VFSTestFileOutput");
    }
} // namespace lce::tests::vfs
