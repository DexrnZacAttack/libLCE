//
// Created by Boreal on 5/20/2025.
//

#include <Filesystem/Filesystem.h>

#include "sstream"

namespace lce::fs {
    Filesystem::Filesystem() {
        this->root = std::make_unique<Directory>(L"/", nullptr); // call the fs root "/"
    }

    Directory* Filesystem::getOrCreateDirByPath(const std::wstring& path) const {
        if (path.empty())
            return nullptr;

        Directory* c = root.get();
        std::wstringstream ss(path);
        std::wstring name;

        while (std::getline(ss, name, L'/')) { // go through the entire path
            if (name.empty())
                continue; // leading / (so we can do /test/test2/test3.png and test/test2/test3.png)
            if (!c || c->isFile())
                return nullptr; // say we want test/test2/test3.png, if test2 is file instead of dir then we're fucked.

            FSObject* child = c->getChild(name); // get the child
            if (!child) {
                child = c->createDirectory(name);
            };

            if (!child->isFile()) { // if it's not a file then we're going down another directory in the tree, unless
                                    // the path ends in a directory
                c = dynamic_cast<Directory*>(child);
            } else { // somehow we found *a* file but aren't at the end of the path... we should not return it since the
                     // path is considered invalid
                return nullptr;
            }
        }

        return c;
    }

    FSObject* Filesystem::getByPath(const std::wstring& path) const {
        if (path.empty())
            return nullptr;

        Directory* c = root.get();
        std::wstringstream ss(path);
        std::wstring name;

        while (std::getline(ss, name, L'/')) { // go through the entire path
            if (name.empty())
                continue; // leading / (so we can do /test/test2/test3.png and test/test2/test3.png)
            if (!c || c->isFile())
                return nullptr; // say we want test/test2/test3.png, if test2 is file instead of dir then we're fucked.

            FSObject* child = c->getChild(name); // get the child
            if (!child)
                return nullptr; // oops! doesn't exist

            if (!child->isFile()) { // if it's not a file then we're going down another directory in the tree, unless
                                    // the path ends in a directory
                c = dynamic_cast<Directory*>(child);
            } else if (ss.eof()) { // we've found the file, and are at the end of the path
                return child;
            } else { // somehow we found *a* file but aren't at the end of the path... we should not return it since the
                     // path is considered invalid
                return nullptr;
            }
        }

        return c;
    }

    void Filesystem::windowsToUnixDelimiter(std::wstring& name) {
        for (wchar_t& c : name) {
            if (c == L'\\') {
                c = L'/';
            }
        }
    }

    void Filesystem::unixToWindowsDelimiter(std::wstring& name) {
        for (wchar_t& c : name) {
            if (c == L'/') {
                c = L'\\';
            }
        }
    }

    FSObject* Filesystem::createFileRecursive(const std::wstring& path, std::vector<uint8_t> data) const {
        if (path.empty())
            return nullptr;

        Directory* current = root.get();
        std::wstringstream ss(path);
        std::wstring name;

        while (std::getline(ss, name, L'/')) {
            if (name.empty())
                continue;

            FSObject* child = current->getChild(name);

            if (!child) {
                if (ss.eof())
                    return current->createFile(name, std::move(data));

                current = current->createDirectory(name);
            } else {
                if (ss.eof())
                    return nullptr; // file already exists

                if (child->isFile())
                    return nullptr; // invalid path
                current = dynamic_cast<Directory*>(child);
            }
        }

        return nullptr; // should never get here
    }


} // namespace lce::fs
