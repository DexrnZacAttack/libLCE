//
// Created by Boreal on 05/20/2025.
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <algorithm>
#include <memory>
#include <vector>

#include <Filesystem/Directory.h>
#include <Filesystem/File.h>
#include <libLCE.h>

namespace lce::fs {

    class LIBLCE_API Filesystem {
      public:
        // const const constexpr const const const
        static constexpr const wchar_t *const ROOT = L"/";

        // hope I did this right
        Filesystem(const Filesystem &f)
            : root(std::make_unique<Directory>(*f.root)) {};

        Filesystem();

        explicit Filesystem(Directory *root);

        /// Gets a directory by path, but if it doesn't already exist, it
        /// creates a new one and returns that.
        [[nodiscard]] Directory *
        getOrCreateDirByPath(const std::wstring &path) const;

        /// Get root directory (/)
        [[nodiscard]] Directory *getRoot() const { return root.get(); };

        /// Get an object by path
        ///
        /// Returns nullptr if it doesn't exist. (or in case of other error)
        [[nodiscard]] FSObject *getByPath(const std::wstring &path) const;

        /// Recursively creates a file (create all directories leading down to
        /// it)
        [[nodiscard]] FSObject *
        createFileRecursive(const std::wstring &path,
                            const std::vector<uint8_t> &data) const;

        /// Converts Windows' path delimiters ("\") to Unix's ("/")
        static void windowsToUnixDelimiter(std::wstring &name);
        /// Converts Unix's path delimiters ("/") to Windows' ("\")
        static void unixToWindowsDelimiter(std::wstring &name);

        [[nodiscard]] virtual std::wstring toString() const {
            return L"Filesystem[fileCount=" +
                   std::to_wstring(root->getFileCount()) +
                   L", directoryCount=" +
                   std::to_wstring(root->getDirectoryCount()) +
                   L", childCount=" +
                   std::to_wstring(root->getFileCount() +
                                   root->getDirectoryCount()) +
                   L", creationTime=" +
                   std::to_wstring(root->getCreationTimestamp()) +
                   L", modifiedTime=" +
                   std::to_wstring(root->getModifiedTimestamp()) + L"]";
        };

        virtual ~Filesystem() = default;

      private:
        std::unique_ptr<Directory> root;
    };
} // namespace lce::fs

#endif // FILESYSTEM_H
