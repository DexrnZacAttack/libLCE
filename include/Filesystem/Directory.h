//
// Created by DexrnZacAttack on 5/27/2025 (ZPC-I1)
//

#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <filesystem>
#include <memory>
#include <unordered_map>

#include <vector>

#include <Filesystem/FSObject.h>
#include <Filesystem/File.h>

namespace lce::fs {

    class LIBLCE_API Directory : public FSObject {
    public:
        explicit Directory(std::wstring name, Directory* parent) : fs::FSObject(std::move(name)) {
            this->parent = parent;
        }

        /// Creates a file with the specified name and data
        File* createFile(const std::wstring& name, std::vector<uint8_t> data);
        /// Creates a directory with the specified name
        Directory* createDirectory(const std::wstring& name);

        /// Gets a child with the specified name (if available)
        ///
        /// If the child is not found, the method will return a nullptr.
        FSObject* getChild(const std::wstring& name) const;

        /// Adds the given FSObject to the directory
        ///
        /// This also sets it's parent and updates it's modification timestamp
        bool addChild(std::unique_ptr<FSObject> child);
        /// Removes/Deletes a child from the directory
        bool removeChild(const std::wstring& name);
        /// Takes a child, which removes it and returns the unique ptr
        ///
        /// NOTE: This will unset the parent
        std::unique_ptr<FSObject> takeChild(const std::wstring& name);

        /// Renames a child
        bool renameChild(const std::wstring& old, const std::wstring& n);

        /// Moves a child to another Directory
        bool moveChild(const std::wstring& name, Directory* to);

        /// Gets the size of the directory and all files under it (recursive)
        size_t getSize() const;

        /// Returns the map of FSObjects under this directory
        const std::unordered_map<std::wstring, std::unique_ptr<FSObject>>& getChildren() const { return children; }

        /// Prints the file listing into stdout
        void printListing();

        /// Gets the file count (recursive)
        [[nodiscard]] size_t getFileCount() const;
        /// Gets the directory count (recursive)
        [[nodiscard]] size_t getDirectoryCount() const;

        /// Gets if the FSObject is a file or not
        ///
        /// Because this is a Directory, it will always return false
        bool isFile() const override { return false; }

        /// Writes the entire directory out to the given path on the user's physical filesystem
        void writeOut(std::filesystem::path path) const;

        /// Writes the entire directory out to the given path on the user's physical filesystem
        void writeOut(const std::wstring& path) const;

        std::wostringstream& operator<<(std::wostringstream& wos) const override {
            wos << L"Directory [name=" << getName() << L", fileCount=" << getFileCount() << L", directoryCount=" << getDirectoryCount() << L", childCount=" << getFileCount() + getDirectoryCount() << L", creationTime=" << getCreationTimestamp() << L", modifiedTime=" << getModifiedTimestamp() << L"]";
            return wos;
        }

        // thanks MSVC
        Directory(const Directory&) = delete;
        Directory& operator=(const Directory&) = delete;
    private:
        std::unordered_map<std::wstring, std::unique_ptr<FSObject>> children;
    };

} // namespace lce::fs

#endif // DIRECTORY_H
