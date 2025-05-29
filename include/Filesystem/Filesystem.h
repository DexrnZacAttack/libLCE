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
        Filesystem();
        /// Gets a directory by path, but if it doesn't already exist, it creates a new one and returns that.
        Directory* getOrCreateDirByPath(const std::wstring& path) const;

        /// Get root directory (/)
        Directory* getRoot() const { return root.get(); };

        /// Get an object by path
        ///
        /// Returns nullptr if it doesn't exist. (or in case of other error)
        FSObject* getByPath(const std::wstring& path) const;

        /// Recursively creates a file (create all directories leading down to it)
        FSObject* createFileRecursive(const std::wstring& path, std::vector<uint8_t> data) const;

        /// Converts Windows' path delimiter ("\") to Unix's ("/")
        static void windowsToUnixDelimiter(std::wstring& name);
        /// Converts Unix path delimiter ("/") to Windows' ("\")
        static void unixToWindowsDelimiter(std::wstring& name);

    private:
        std::unique_ptr<Directory> root;
    };
} // namespace lce::fs

#endif // FILE_H
