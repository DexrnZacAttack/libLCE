//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#include "vfs.h"

#include "Filesystem/Directory.h"
#include "Filesystem/Filesystem.h"

namespace lce::tests::vfs {
    void runForEachRecursive() {
        const std::filesystem::path p(util::examples / "VFSTestFolder");
        const fs::Directory *d = new fs::Directory(p);

        std::cout << "Reading " << p << std::endl;
        std::cout << "Found " << d->getDirectoryCount() + d->getFileCount()
                  << " children ";
        std::cout << "(" << d->getFileCount() << " files, ";
        std::cout << d->getDirectoryCount() << " directories)" << std::endl;

        d->forEachRecursive([](const std::wstring &name,
                               const fs::FSObject &f) {
            if (f.isFile()) {
                std::wcout << dynamic_cast<const fs::File &>(f) << std::endl;
            } else {
                std::wcout << dynamic_cast<const fs::Directory &>(f)
                           << std::endl;
            }
        });
    }

    void runForEachFilesOnlyRecursive() {
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEachFilesRecursive(
            [](const std::wstring &name, const fs::File &f) {
                std::wcout << f << std::endl;
            });
    }

    void runForEachDirectoriesOnlyRecursive() {
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEachDirectoriesRecursive(
            [](const std::wstring &name, const fs::Directory &f) {
                std::wcout << f << std::endl;
            });
    }
    void runForEach() {
        std::filesystem::create_directories(util::examples / "VFSTestFolder");
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEach([](const std::wstring &name, const fs::FSObject &f) {
            if (f.isFile()) {
                std::wcout << dynamic_cast<const fs::File &>(f) << std::endl;
            } else {
                std::wcout << dynamic_cast<const fs::Directory &>(f)
                           << std::endl;
            }
        });
    }

    void runForEachFilesOnly() {
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEachFiles([](const std::wstring &name, const fs::File &f) {
            std::wcout << f << std::endl;
        });
    }

    void runForEachDirectoriesOnly() {
        const fs::Directory *d =
            new fs::Directory(util::examples / "VFSTestFolder");

        d->forEachDirectories(
            [](const std::wstring &name, const fs::Directory &f) {
                std::wcout << f << std::endl;
            });
    }

    void createFileFromReal() {
        const fs::File file(util::examples / "VFSTestFile");

        std::wcout << file << std::endl;

        file.writeOut(util::output / "VFSTestFileOutput");
    }
} // namespace lce::tests::vfs
