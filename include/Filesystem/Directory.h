//
// Created by DexrnZacAttack on 5/27/2025 (ZPC-I1)
//

#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <filesystem>
#include <functional>
#include <memory>
#include <stack>
#include <unordered_map>

#include <vector>

#include <Filesystem/FSObject.h>
#include <Filesystem/File.h>

namespace lce::fs {

    /** Contains other `FSObject`s inside it.
     *
     * Can be used as the root of a Filesystem
     *
     * @see File
     * @see Filesystem
     * @see FSObject
     */
    class LIBLCE_API Directory : public FSObject {
      public:
        Directory(Directory &&) = default;
        Directory &operator=(Directory &&) = default;

        /** Creates a Directory
         *
         * @param name The name of the directory (if root, specify
         * Filesystem::ROOT)
         * @param parent The parent directory (if root, specify none)
         */
        explicit Directory(std::wstring name, Directory *parent)
            : fs::FSObject(std::move(name)) {
            this->parent = parent;
        }

        bool contains(const std::wstring &name) const {
            return this->children.count(name);
        }

        /** Creates a Directory with the contents of a physical directory path
         *
         * @param path The path to read all files into the directory from
         */
        explicit Directory(const std::filesystem::path &path);

        /** Creates a file with the specified name and data
         *
         * @param name The name you'd like to give to the newly created file
         * @param data The data that the file will hold
         */
        File *createFile(const std::wstring &name,
                         const std::vector<uint8_t> &data);
        FSObject *createFileRecursive(const std::wstring &path,
                                      const std::vector<uint8_t> &data);
        /** Creates a directory with the specified name
         *
         * @param name The name you'd like to give to the newly created
         * directory
         */
        Directory *createDirectory(const std::wstring &name);

        /** Gets a child with the specified name (if available)
         *
         * @param name The name of the child you want to get
         *
         * If the child is not found, the method will return a nullptr.
         */
        FSObject *getChild(const std::wstring &name) const;

        /** Calls a method for each file in directory (recursive)
         *
         * @param l The function/lambda to call on all files
         */
        void forEachFilesRecursive(
            const std::function<void(std::wstring name, File &file)> &l) const;

        /** Calls a method for each directory in directory (recursive)
         *
         * @param l The function/lambda to call on all directories
         */
        void forEachDirectoriesRecursive(
            const std::function<void(std::wstring name, Directory &dir)> &l)
            const;

        /** Calls a method for each object in directory (recursive)
         *
         * @param l The function/lambda to call on all objects (file, folder)
         */
        void forEachRecursive(
            const std::function<void(std::wstring name, FSObject &obj)> &l)
            const;

        /** Calls a method for each file in directory
         *
         * @param l The function/lambda to call on all files
         */
        void forEachFiles(
            const std::function<void(std::wstring name, File &file)> &l) const;

        /** Calls a method for each directory in directory
         *
         * @param l The function/lambda to call on all directories
         */
        void forEachDirectories(
            const std::function<void(std::wstring name, Directory &dir)> &l)
            const;

        /** Calls a method for each object in directory
         *
         * @param l The function/lambda to call on all objects (file, folder)
         */
        void forEach(const std::function<void(std::wstring name, FSObject &obj)>
                         &l) const;

        /** Adds the given FSObject to the directory
         *
         * This also sets it's parent and updates it's modification timestamp
         *
         * @param child The child FSObject you would like too add
         */
        bool addChild(std::unique_ptr<FSObject> child);

        /** Removes/Deletes a child from the directory
         *
         * @param name The name of the child you would like to remove
         */
        bool removeChild(const std::wstring &name);

        /** Takes a child, which removes it and returns the unique ptr
         *
         * NOTE: This will unset the parent
         *
         * @param name The name of the child you would like to take
         */
        std::unique_ptr<FSObject> takeChild(const std::wstring &name);

        /** Renames a child
         *
         * @param child The name of the child you would like to rename
         * @param n The new name to give to the child
         */
        bool renameChild(const std::wstring &child, const std::wstring &n);

        /** Moves a child to another Directory
         *
         * @param name The name of the child you would like to move
         * @param to The directory you would like to move the child to
         */
        bool moveChild(const std::wstring &name, Directory *to);

        /** Gets the size of the directory and all files under it (recursive) */
        size_t getSize() const override;

        /** Returns the map of FSObjects under this directory */
        const std::unordered_map<std::wstring, std::unique_ptr<FSObject>> &
        getChildren() const {
            return children;
        }

        /** Prints the file listing into stdout */
        void printListing();

        /** Gets the file count (recursive) */
        [[nodiscard]] size_t getFileCount() const;
        /** Gets the directory count (recursive) */
        [[nodiscard]] size_t getDirectoryCount() const;

        /** Gets if the FSObject is a file or not
         *
         * Because this is a Directory, it will always return false
         */
        bool isFile() const override { return false; }

        /** Writes the entire directory out to the given path on the user's
         * physical filesystem
         *
         * @param path The physical path to write into
         */
        void writeOut(const std::filesystem::path &path) const;

        /** Writes the entire directory out to the given path on the user's
         * physical filesystem
         *
         * @param path The physical path to write into
         */
        void writeOut(const std::wstring &path) const;

        std::wstring toString() const override {
            return L"Directory[name=" + this->getName() + L", fileCount=" +
                   std::to_wstring(this->getFileCount()) +
                   L", directoryCount=" +
                   std::to_wstring(this->getDirectoryCount()) +
                   L", childCount=" +
                   std::to_wstring(this->getFileCount() +
                                   this->getDirectoryCount()) +
                   L", creationTime=" +
                   std::to_wstring(this->getCreationTimestamp()) +
                   L", modifiedTime=" +
                   std::to_wstring(this->getModifiedTimestamp()) + L"]";
        };

        friend std::wostream &operator<<(std::wostream &wos,
                                         const Directory &d) {
            wos << d.toString();
            return wos;
        }

        friend std::wostream &operator<<(std::wostream &wos,
                                         const Directory *d) {
            wos << d->toString();
            return wos;
        }

        // THIS WAS PAIN IN THE ASS I NEVER WANT TO DO THIS AGAIN NO
        Directory(const Directory &d)
            : FSObject(d.name, d.creationTime, d.modifiedTime, d.parent) {
            for (auto &[n, child] : d.children) {
                // god, I feel like this will probably be really slow
                // why does it use copy constructor anyway for making derived
                // object based off of existing base instance
                if (const Directory *dd =
                        dynamic_cast<Directory *>(child.get())) {
                    children[n] = std::make_unique<Directory>(*dd);
                } else if (const File *f = dynamic_cast<File *>(child.get())) {
                    children[n] = std::make_unique<File>(*f);
                }

                children[n]->parent = this;
            }
        };

      private:
        /** Child objects inside the Directory */
        std::unordered_map<std::wstring, std::unique_ptr<FSObject>> children;
    };

} // namespace lce::fs

#endif // DIRECTORY_H
