//
// Created by DexrnZacAttack on 9/5/25 using zPc-i2.
//
#include "LCE/filesystem/File.h"

#include <filesystem>

#include "LCE/filesystem/Directory.h"

namespace lce::fs {
    void File::writeOut(const std::filesystem::path &path,
                        const FSObject::name_t &name) const {
        std::filesystem::create_directories(path);

        std::ofstream os(path / name);
        os << this;
        os.close();

        // TODO: write time and other metadata
    }
    void File::writeOut(const FSObject::path_t &path,
                        const FSObject::name_t &name) const {
        writeOut(std::filesystem::path(path), name);
    }

    void File::writeOut(const FSObject::path_t &path) const {
        writeOut(path, getName());
    };

    void File::writeOutFullPath(const FSObject::path_t &path) const {
        const std::filesystem::path p(path);

        writeOut(p.parent_path(), p.filename().string());
    };

    void File::writeOutFullPath(const std::filesystem::path &path) const {
        writeOut(path.parent_path(), path.filename().string());
    };

    void File::writeOut(const std::filesystem::path &path) const {
        writeOut(path, getName());
    }

    void File::writeOut() const {
        writeOut(std::filesystem::current_path(), getName());
    }
    void File::writeOutWithDirs() const {
        writeOut(std::filesystem::current_path() /
                     std::filesystem::path(getPath().substr(1)).parent_path(),
                 getName());
    }

    void File::writeOutWithDirs(const FSObject::path_t &path) const {
        writeOut(path /
                     std::filesystem::path(getPath().substr(1)).parent_path(),
                 getName());
    }

    void File::writeOutWithDirs(const std::filesystem::path &path) const {
        writeOut(path /
                     std::filesystem::path(getPath().substr(1)).parent_path(),
                 getName());
    }
    void File::writeOutWithDirs(const FSObject::path_t &path,
                                const FSObject::name_t &name) const {
        writeOut(path /
                     std::filesystem::path(getPath().substr(1)).parent_path(),
                 name);
    }

    void File::writeOutWithDirs(const std::filesystem::path &path,
                                const FSObject::name_t &name) const {
        writeOut(path /
                     std::filesystem::path(getPath().substr(1)).parent_path(),
                 name);
    }
} // namespace lce::fs