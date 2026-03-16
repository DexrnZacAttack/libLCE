//
// Created by DexrnZacAttack on 5/27/2025.
//

#ifndef FILE_H
#define FILE_H

#include "LCE/libLCE.h"
#include <filesystem>
#include <string>

#include <vector>

#include "LCE/filesystem/FSObject.h"
#include "fstream"

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
        File(FSObject::name_t name, std::vector<uint8_t> data, Directory *parent)
            : FSObject(std::move(name)), m_data(std::move(data)) {
            this->m_parent = parent;
        }

      public:
        File(FSObject::name_t name, std::vector<uint8_t> data)
            : FSObject(std::move(name)), m_data(std::move(data)) {}

        /** Creates a File from a physical file on the user's filesystem */
        explicit File(const std::filesystem::path &path)
            : FSObject(path.filename().string()) {
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

            this->m_data =
                std::vector<uint8_t>(std::filesystem::file_size(path));
            name.read(reinterpret_cast<char *>(this->m_data.data()),
                      this->m_data.size());
        }

        File(File &&) = default;
        File &operator=(File &&) = default;

        /// Writes the file to the physical filesystem into the given path with
        /// the given filename
        void writeOut(const std::filesystem::path &path,
                      const FSObject::name_t &name) const;

        /// Writes the file to the physical filesystem into the given path with
        /// the given filename
        void writeOut(const FSObject::path_t &path, const FSObject::name_t &name) const;

        /// Writes the file to the physical filesystem into the given path
        void writeOut(const FSObject::path_t &path) const;

        /// Writes the file to the physical filesystem into the given path
        void writeOut(const std::filesystem::path &path) const;

        /// Writes the file to the physical filesystem into the given path
        /// (using end of path as filename)
        void writeOutFullPath(const FSObject::path_t &path) const;

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
        void writeOutWithDirs(const FSObject::path_t &path) const;

        /// Writes the file and parent directories to the physical filesystem
        /// into the given path
        void writeOutWithDirs(const std::filesystem::path &path) const;

        /// Writes the file and parent directories to the physical filesystem
        /// into the given path with the given filename
        void writeOutWithDirs(const FSObject::path_t &path,
                              const FSObject::name_t &name) const;

        /// Writes the file and parent directories to the physical filesystem
        /// into the given path with the given filename
        void writeOutWithDirs(const std::filesystem::path &path,
                              const FSObject::name_t &name) const;

        /// Gets if the FSObject is a file or not
        ///
        /// Because this is a File, it will always return true
        [[nodiscard]] bool isFile() const override { return true; }

        /// Gets the file's data
        [[nodiscard]] const std::vector<uint8_t> &getData() const {
            return m_data;
        }
        /// Gets the file's size (equiv. getData().size())
        size_t getSize() const override { return m_data.size(); }

        [[nodiscard]] FSObject::string_t toString() const override {
            return "File[name=" + this->getName() + ", size=" +
                   std::to_string(this->getSize()) + ", creationTime=" +
                   std::to_string(this->getCreationTimestamp()) +
                   ", modifiedTime=" +
                   std::to_string(this->getModifiedTimestamp()) + "]";
        }

        /// Output file data
        friend std::ostream &operator<<(std::ostream &os, const File &f) {
            os.write(reinterpret_cast<const char *>(f.m_data.data()),
                     f.m_data.size());
            return os;
        }

        /// Output file data
        friend std::ostream &operator<<(std::ostream &os, const File *f) {
            os.write(reinterpret_cast<const char *>(f->m_data.data()),
                     f->m_data.size());
            return os;
        }

        /// Output file info as string
        friend FSObject::ostringstream_t &operator<<(FSObject::ostringstream_t &wos, const File &f) {
            wos << f.toString();
            return wos;
        }

        /// Output file info as string
        friend FSObject::ostringstream_t &operator<<(FSObject::ostringstream_t &wos, const File *f) {
            wos << f->toString();
            return wos;
        }

        /// Push a byte into the file
        File &operator<<(const uint8_t b) {
            m_data.push_back(b);
            return *this;
        }

        /// Read/Write a byte from the file
        uint8_t &operator[](const int i) { return m_data[i]; }

        /// Read a byte from the file
        uint8_t operator[](const int i) const { return m_data[i]; }

        /// Overwrites all data in the file with the given data
        void setData(const std::vector<uint8_t> &d) { this->m_data = d; }

        File(const File &c)
            : FSObject(c.m_name, c.m_creationTime, c.m_modifiedTime, c.m_parent),
              m_data(c.m_data) {}

      private:
        std::vector<uint8_t> m_data;
    };
} // namespace lce::fs

#endif
