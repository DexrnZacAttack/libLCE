//
// Created by DexrnZacAttack on 5/27/2025 (ZPC-I1)
//

#include "Directory.h"

#include "File.h"

namespace lce::fs {
    File *Directory::createFile(const std::wstring& name, std::vector<uint8_t> data) {
        if (children.count(name)) return nullptr; // file exists already... can't have 2 of them (although we are NOT case-insensitive)

        std::unique_ptr<File> f = std::make_unique<File>(name, data, this);
        File* ptr = f.get();
        children[name] = std::move(f);

        return ptr;
    }

    Directory *Directory::createDirectory(const std::wstring& name) {
        if (children.count(name)) return nullptr; // already exists

        std::unique_ptr<Directory> dir = std::make_unique<Directory>(name, this);
        Directory* ptr = dir.get();
        children[name] = std::move(dir);

        return ptr;
    }

    FSObject* Directory::getChild(const std::wstring& name) const {
        const auto it = children.find(name); // const auto
        if (it == children.end()) return nullptr; // does not exist

        return it->second.get();
    }

    /// Allows us to add any type of object that extends FSObject by relying on us to make the object instead of the Directory class
    /// Useful, although it does break the "no orphaned file should exist" guideline that I wanted to follow...
    /// However, I think things such as takeChild also break that, as technically you are the sole owner of the child that you took.
    bool Directory::addChild(std::unique_ptr<FSObject> child)
    {
        std::wstring name = child->getName();

        if (children.count(name)) return false; // exists

        child->parent = this;
        children[name] = std::move(child);
        return true;
    }

    bool Directory::removeChild(const std::wstring& name) {
        return children.erase(name);
    }

    std::unique_ptr<FSObject> Directory::takeChild(const std::wstring& name) {
        auto it = children.find(name);

        if (it == children.end()) return nullptr; // does not exist

        std::unique_ptr<FSObject> obj = std::move(it->second);
        obj->parent = nullptr;
        children.erase(it); // nuke it from the dir (since whatever took it now has full control over it)
        return obj;
    }

    bool Directory::renameChild(const std::wstring& old, const std::wstring& n) { // thanks for taking `new`
        if (old == n || children.count(n)) return false; // if old is the same or new name already exists

        std::unique_ptr<FSObject> obj = takeChild(old);
        if (!obj) return false; // does not exist

        obj->setName(n); // set the name

        children[n] = std::move(obj); // move the child
        return true;
    }

    bool Directory::moveChild(const std::wstring& name, Directory* to) {
        if (!to || to == this || to->children.count(name)) return false; // if to is nullptr, or to is literally this, or to contains an object with the same name already

        std::unique_ptr<FSObject> obj = takeChild(name); // take from this one
        if (!obj) return false;

        obj->parent = to;
        obj->modifiedTime = lce::system::getTimestamp();
        to->children[name] = std::move(obj); // drop it into the new dir
        return true;
    }

    size_t Directory::getSize() const {
        size_t s = 0;

        for (const auto& [name, child] : children) {
            if (!child->isFile())
                if (const Directory* dir = dynamic_cast<const Directory*>(child.get())) s += dir->getSize();

            if (const File* file = dynamic_cast<const File*>(child.get())) s += file->getSize();
        }

        return s;
    }

    void Directory::printListing()
    {
        for (const auto& [name, child] : children) {
            if (!child->isFile()) {
                if (Directory *dir = dynamic_cast<Directory*>(child.get())) {
                    std::wcout << L"[D | " << dir->getSize() << L"] " << dir->getName() << std::endl;
                }
            } else
            {
                if (File *f = dynamic_cast<File*>(child.get())) {
                    std::wcout << L"[F | " << f->getSize() << L"] " << f->getName() << std::endl;
                }
            }
        }
    }

    size_t Directory::getFileCount() const {
        size_t i = 0;
        for (const auto& [name, child] : children) {
            if (!child->isFile())
                if (Directory *dir = dynamic_cast<Directory*>(child.get())) i += dir->getFileCount(); // infinite recursion

            i++;
        }
        return i;
    }

    size_t Directory::getDirectoryCount() const {
        size_t i = 0;
        for (const auto& [name, child] : children) {
            if (!child->isFile()) {
                if (Directory *dir = dynamic_cast<Directory*>(child.get())) {
                    i += dir->getDirectoryCount();
                }
            }
        }
        return i;
    }

}
