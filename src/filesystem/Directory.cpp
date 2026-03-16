//
// Created by DexrnZacAttack on 5/27/2025 (ZPC-I1)
//

#include "LCE/filesystem/Directory.h"
#include "LCE/filesystem/File.h"

#include <filesystem>
#include <fstream>
#include <ranges>
#include <stack>

namespace lce::fs {
    Directory::Directory(const std::filesystem::path &path)
        : FSObject(path.filename().string()) {
        this->m_parent = nullptr;

        for (const auto &o :
             std::filesystem::recursive_directory_iterator(path)) {
            if (o.is_regular_file()) {
                FSObject::path_t a =
                    std::filesystem::relative(o.path(), path).string();

                std::ifstream name(o.path(), std::ifstream::binary);
                if (!name.is_open())
                    throw std::ios_base::failure(
                        std::string("Failed to open file ") +
                        (o.path()).string());
                std::vector<uint8_t> out(std::filesystem::file_size(o.path()));
                name.read(reinterpret_cast<char *>(out.data()), out.size());

                createFileRecursive(a, std::move(out));
            }
        }
    }

    File *Directory::createFile(const FSObject::name_t &name,
                                const std::vector<uint8_t> &&data) {
        // should we except instead of just returning?
        if (children.contains(name))
            return nullptr; // file exists already... can't have 2 of them
                            // (although we are NOT case-insensitive)

        std::unique_ptr<File> f =
            std::unique_ptr<File>(new File(name, std::move(data), this));

        File *ptr = f.get();
        children[name] = std::move(f);

        return ptr;
    }

    FSObject *Directory::createFileRecursive(const FSObject::path_t &path,
                                             const std::vector<uint8_t> &&data) {
        if (path.empty())
            return nullptr;

        Directory *current = this;
        std::basic_stringstream<FSObject::char_t> ss(path);
        FSObject::name_t name;

        while (std::getline(ss, name, '/')) {
            if (name.empty())
                continue;

            // todo: handle current nullptr
            FSObject *child = current->getChild(name);

            if (!child) {
                if (ss.eof())
                    return current->createFile(name, std::move(data));

                current = current->createDirectory(name);
            } else {
                if (ss.eof())
                    return nullptr; // file already exists

                if (child->isFile())
                    return nullptr; // invalid path
                current = dynamic_cast<Directory *>(child);
            }
        }

        return nullptr; // should never get here
    }

    Directory *Directory::createDirectory(const FSObject::name_t &name) {
        if (children.count(name))
            return nullptr; // already exists

        std::unique_ptr<Directory> dir =
            std::make_unique<Directory>(name, this);
        Directory *ptr = dir.get();
        children[name] = std::move(dir);

        return ptr;
    }

    FSObject *Directory::getChild(const FSObject::name_t &name) const {
        const auto it = children.find(name); // const auto
        if (it == children.end())
            return nullptr; // does not exist

        return it->second.get();
    }

    void Directory::forEachFilesRecursive(
        const std::function<void(FSObject::name_t name, File &file)> &l) const {
        std::stack<const Directory *> stack;
        stack.push(this);

        while (!stack.empty()) {
            const Directory *d = stack.top();
            stack.pop();

            for (const auto &[n, child] : d->getChildren()) {
                if (!child->isFile()) {
                    stack.push(dynamic_cast<const Directory *>(child.get()));
                    continue;
                }

                File *f = dynamic_cast<fs::File *>(child.get());
                l(n, *f);
            }
        }
    }

    void Directory::forEachDirectoriesRecursive(
        const std::function<void(FSObject::name_t name, Directory &dir)> &l) const {
        std::stack<const Directory *> stack;
        stack.push(this);

        while (!stack.empty()) {
            const Directory *d = stack.top();
            stack.pop();

            for (const auto &[n, child] : d->getChildren()) {
                if (child->isFile()) {
                    continue;
                }

                Directory *dd = dynamic_cast<fs::Directory *>(child.get());
                stack.push(
                    dd); // this might break if the user modifes the Directory
                         // pointer when we get to it on the stack

                l(n, *dd);
            }
        }
    }

    void Directory::forEachRecursive(
        const std::function<void(FSObject::name_t name, FSObject &obj)> &l) const {
        std::stack<const Directory *> stack;
        stack.push(this);

        while (!stack.empty()) {
            const Directory *d = stack.top();
            stack.pop();

            for (const auto &[n, child] : d->getChildren()) {
                if (child->isFile()) {
                    File *s = dynamic_cast<fs::File *>(child.get());
                    l(n, *s);

                    continue;
                }

                Directory *dd = dynamic_cast<fs::Directory *>(child.get());
                stack.push(
                    dd); // this might break if the user modifes the Directory
                         // pointer when we get to it on the stack

                l(n, *dd);
            }
        }
    }

    void Directory::forEachFiles(
        const std::function<void(FSObject::name_t name, File &file)> &l) const {
        for (auto &[n, c] : children) {
            if (c->isFile())
                l(n, *dynamic_cast<File *>(c.get()));
        }
    }

    void Directory::forEachDirectories(
        const std::function<void(FSObject::name_t name, Directory &dir)> &l) const {
        for (auto &[n, c] : children) {
            if (!c->isFile())
                l(n, *dynamic_cast<Directory *>(c.get()));
        }
    }

    void Directory::forEach(
        const std::function<void(FSObject::name_t name, FSObject &obj)> &l) const {
        for (auto &[n, c] : this->children) {
            l(n, *c.get());
        }
    }

    bool Directory::addChild(std::unique_ptr<FSObject> child) {
        const FSObject::name_t name = child->getName();

        if (children.contains(name))
            return false; // exists

        child->m_parent = this;
        children[name] = std::move(child);
        return true;
    }

    bool Directory::removeChild(const FSObject::name_t &name) {
        return children.erase(name);
    }

    std::unique_ptr<FSObject> Directory::takeChild(const FSObject::name_t &name) {
        const auto it = children.find(name);

        if (it == children.end())
            return nullptr; // does not exist

        std::unique_ptr<FSObject> obj = std::move(it->second);
        obj->m_parent = nullptr;
        children.erase(it); // nuke it from the dir (since whatever took it now
                            // has full control over it)
        return obj;
    }

    bool
    Directory::renameChild(const FSObject::name_t &child,
                           const FSObject::name_t &n) { // thanks for taking `new`
        if (child == n || children.contains(n))
            return false; // if old is the same or new name already exists

        std::unique_ptr<FSObject> obj = takeChild(child);
        if (!obj)
            return false; // does not exist

        obj->setName(n); // set the name

        children[n] = std::move(obj); // move the child
        return true;
    }

    bool Directory::moveChild(const FSObject::name_t &name, Directory *to) {
        if (!to || to == this || to->children.contains(name))
            return false; // if `to` is nullptr, or `to` is literally this, or
                          // `to` contains an object with the same name already

        std::unique_ptr<FSObject> obj = takeChild(name); // take from this one
        if (!obj)
            return false;

        obj->m_parent = to;
        obj->m_modifiedTime = lce::system::getTimestamp();
        to->children[name] = std::move(obj); // drop it into the new dir
        return true;
    }

    size_t Directory::getSize() const {
        size_t s = 0;

        for (const auto &child : children | std::views::values) {
            if (!child->isFile())
                if (const Directory *dir =
                        dynamic_cast<const Directory *>(child.get()))
                    s += dir->getSize();

            if (const File *file = dynamic_cast<const File *>(child.get())) {
                s += file->getSize();
            }
        }

        return s;
    }

    void Directory::printListing() {
        for (const auto &[name, child] : children) {
            if (!child->isFile()) {
                if (const Directory *d =
                        dynamic_cast<Directory *>(child.get())) {
                    std::cout << "[D | " << d->getSize() << "] "
                               << d->getName() << std::endl;
                }
            } else {
                if (const File *f = dynamic_cast<File *>(child.get())) {
                    std::cout << "[F | " << f->getSize() << "] "
                               << f->getName() << std::endl;
                }
            }
        }
    }

    size_t Directory::getFileCount() const {
        size_t i = 0;
        for (const auto &[name, child] : children) {
            if (!child->isFile()) {
                if (const Directory *dir =
                        dynamic_cast<Directory *>(child.get()))
                    i += dir->getFileCount(); // infinite recursion
            } else {
                i++;
            }
        }
        return i;
    }

    size_t Directory::getDirectoryCount() const {
        size_t i = 0;
        for (const auto &[name, child] : children) {
            if (!child->isFile()) {
                if (const Directory *dir =
                        dynamic_cast<Directory *>(child.get())) {
                    i++;
                    i += dir->getDirectoryCount(); // infinite recursion
                }
            }
        }
        return i;
    }

    void Directory::writeOut(const std::filesystem::path &path) const {
        // would it be bad to use name here?
        forEachFilesRecursive([path](const FSObject::name_t &name, const File &f) {
            f.writeOutWithDirs(path, f.getName());
        });
    }

    void Directory::writeOut(const FSObject::path_t &path) const {
        this->writeOut(std::filesystem::path(path));
    }
} // namespace lce::fs
