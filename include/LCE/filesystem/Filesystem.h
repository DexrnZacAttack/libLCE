//
// Created by Boreal on 05/20/2025.
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <algorithm>
#include <memory>
#include <vector>

#include "LCE/filesystem/Directory.h"
#include "LCE/filesystem/File.h"
#include "LCE/libLCE.h"

namespace lce::fs {

    // TODO I would like to move this to it's own lib, and make use of file
    // streams and such this could allow for one interface for both physical and
    // virtual files. e.g. having a file that holds a handle/stream of a file on
    // the user's physical hard disk.

    /** Contains a root folder and helper methods for traversing the filesystem
     *
     * Can be extended to make custom filesystem types
     */
    class LIBLCE_API Filesystem {
      public:
        // const const constexpr const const const
        /** Root directory path */
        static constexpr const FSObject::char_t *const ROOT = "/";

        // hope I did this right
        Filesystem(const Filesystem &f)
            : root(std::make_unique<Directory>(*f.root)) {};

        Filesystem();

        explicit Filesystem(Directory *root);

        /// Gets a directory by path, but if it doesn't already exist, it
        /// creates a new one and returns that.
        [[nodiscard]] Directory *
        getOrCreateDirByPath(const FSObject::path_t &path) const;

        /// Get root directory (/)
        [[nodiscard]] Directory *getRoot() const { return root.get(); };

        /// Get an object by path
        ///
        /// Returns nullptr if it doesn't exist. (or in case of other error)
        [[nodiscard]] FSObject *getByPath(const FSObject::path_t &path) const;

        /// Recursively creates a file (create all directories leading down to
        /// it)
        FSObject *createFileRecursive(const FSObject::path_t &path,
                                      const std::vector<uint8_t> &data) const;

        /// Converts Windows' path delimiters ("\") to Unix's ("/")
        static FSObject::path_t windowsToUnixDelimiter(const FSObject::path_t &name);
        /// Converts Unix's path delimiters ("/") to Windows' ("\")
        static FSObject::path_t unixToWindowsDelimiter(const FSObject::path_t &name);

        [[nodiscard]] virtual FSObject::string_t toString() const {
            return "Filesystem[fileCount=" +
                   std::to_string(root->getFileCount()) +
                   ", directoryCount=" +
                   std::to_string(root->getDirectoryCount()) +
                   ", childCount=" +
                   std::to_string(root->getFileCount() +
                                   root->getDirectoryCount()) +
                   ", creationTime=" +
                   std::to_string(root->getCreationTimestamp()) +
                   ", modifiedTime=" +
                   std::to_string(root->getModifiedTimestamp()) + "]";
        };

        virtual ~Filesystem() = default;

      private:
        /** The root directory of the filesystem */
        std::unique_ptr<Directory> root;
    };
} // namespace lce::fs

#endif // FILESYSTEM_H
