//
// Created by DexrnZacAttack on 5/27/2025.
//

#ifndef FILE_H
#define FILE_H

#include <filesystem>
#include <libLCE.h>
#include <string>

#include <vector>

#include "fstream"
#include <Filesystem/FSObject.h>

namespace lce::fs {
    /** Holds data
     *
     * @see Directory
     * @see FSObject
     */
    class LIBLCE_API File : public FSObject {
      protected:
        friend class Directory;

        // TODO: allowing for Parent to be set detaches the file from the actual
        // directory doesn't it??? like the children map isn't synced
        File(std::wstring name, std::vector<uint8_t> data, Directory *parent)
            : FSObject(std::move(name)), data(std::move(data)) {
            this->parent = parent;
        }

      public:
        File(std::wstring name, std::vector<uint8_t> data)
            : FSObject(std::move(name)), data(std::move(data)) {}

        /** Creates a File from a physical file on the user's filesystem */
        explicit File(const std::filesystem::path &path)
            : FSObject(path.filename().wstring()) {
            if (!std::filesystem::exists(path) ||
                !std::filesystem::is_regular_file(path)) {
                throw std::runtime_error(
                    "Could not read file '" + (path).string() +
                    "' into new File because file does not exist or is not a "
                    "file"); // confusing as all hell, I'll fix it one
                             // day
            }

            // TODO: we could do timestamp stuff but I think I would need to
            // make a shit ton of ifdefs for os specific apis :sob:

            std::ifstream name(path, std::ifstream::binary);

            if (!name.is_open())
                throw std::ios_base::failure(
                    std::string("Failed to open file ") + (path).string());

            this->data = std::vector<uint8_t>(std::filesystem::file_size(path));
            name.read(reinterpret_cast<char *>(this->data.data()),
                      this->data.size());
        }

        File(File &&) = default;
        File &operator=(File &&) = default;

        /// Writes the file to the physical filesystem into the given path with
        /// the given filename
        void writeOut(const std::filesystem::path &path,
                      const std::wstring &name) const;

        /// Writes the file to the physical filesystem into the given path with
        /// the given filename
        void writeOut(const std::wstring &path, const std::wstring &name) const;

        /// Writes the file to the physical filesystem into the given path
        void writeOut(const std::wstring &path) const;

        /// Writes the file to the physical filesystem into the given path
        void writeOut(const std::filesystem::path &path) const;

        /// Writes the file to the physical filesystem into the given path
        /// (using end of path as filename)
        void writeOutFullPath(const std::wstring &path) const;

        /// Writes the file to the physical filesystem into the given path
        /// (using end of path as filename)
        void writeOutFullPath(const std::filesystem::path &path) const;

        /// Writes the file to the physical filesystem to the current dir
        void writeOut() const;

        /// Writes the file and parent directories to the physical filesystem in
        /// the current directory
        void writeOutWithDirs() const;

        /// Writes the file and parent directories to the physical filesystem
        /// into the given path
        void writeOutWithDirs(const std::wstring &path) const;

        /// Writes the file and parent directories to the physical filesystem
        /// into the given path
        void writeOutWithDirs(const std::filesystem::path &path) const;

        /// Writes the file and parent directories to the physical filesystem
        /// into the given path with the given filename
        void writeOutWithDirs(const std::wstring &path,
                              const std::wstring &name) const;

        /// Writes the file and parent directories to the physical filesystem
        /// into the given path with the given filename
        void writeOutWithDirs(const std::filesystem::path &path,
                              const std::wstring &name) const;

        /// Gets if the FSObject is a file or not
        ///
        /// Because this is a File, it will always return true
        [[nodiscard]] bool isFile() const override { return true; }

        /// Gets the file's data
        [[nodiscard]] const std::vector<uint8_t> &getData() const {
            return data;
        }
        /// Gets the file's size (equiv. getData().size())
        size_t getSize() const override { return data.size(); }

        [[nodiscard]] std::wstring toString() const override {
            return L"File[name=" + this->getName() + L", size=" +
                   std::to_wstring(this->getSize()) + L", creationTime=" +
                   std::to_wstring(this->getCreationTimestamp()) +
                   L", modifiedTime=" +
                   std::to_wstring(this->getModifiedTimestamp()) + L"]";
        };

        /// Output file data
        friend std::ostream &operator<<(std::ostream &os, const File &f) {
            os.write(reinterpret_cast<const char *>(f.data.data()),
                     f.data.size());
            return os;
        }

        /// Output file data
        friend std::ostream &operator<<(std::ostream &os, const File *f) {
            os.write(reinterpret_cast<const char *>(f->data.data()),
                     f->data.size());
            return os;
        }

        /// Output file info as string
        friend std::wostream &operator<<(std::wostream &wos, const File &f) {
            wos << f.toString();
            return wos;
        }

        /// Output file info as string
        friend std::wostream &operator<<(std::wostream &wos, const File *f) {
            wos << f->toString();
            return wos;
        }

        /// Push a byte into the file
        File &operator<<(const uint8_t b) {
            data.push_back(b);
            return *this;
        }

        /// Read/Write a byte from the file
        uint8_t &operator[](const int i) { return data[i]; }

        /// Read a byte from the file
        uint8_t operator[](const int i) const { return data[i]; }

        /// Overwrites all data in the file with the given data
        void setData(const std::vector<uint8_t> &d) { this->data = d; }

        File(const File &c)
            : FSObject(c.name, c.creationTime, c.modifiedTime, c.parent),
              data(c.data) {}

      private:
        std::vector<uint8_t> data;
    };
} // namespace lce::fs

#endif
