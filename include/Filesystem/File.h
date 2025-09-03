//
// Created by DexrnZacAttack on 5/27/2025.
//

#ifndef FILE_H
#define FILE_H
#include <libLCE.h>
#include <string>

#include <vector>

#include <Filesystem/FSObject.h>
#include <sstream>
#include <fstream>

namespace lce::fs {

    class LIBLCE_API File : public FSObject {
    public:
        File(std::wstring name, std::vector<uint8_t> data, Directory* parent) :
            FSObject(std::move(name)), data(std::move(data)) {
            this->parent = parent;
        }

        // TODO: read from fs

        /// Writes the file to the physical filesystem into the given path with the given filename
        void writeOut(std::filesystem::path path, const std::wstring &name) const {
            std::filesystem::path filePath = path / name;

            std::filesystem::create_directories(path);

            std::ofstream os(filePath);
            os << this;
            os.close();

            // TODO: write time
        };

        /// Writes the file to the physical filesystem into the given path with the given filename
        void writeOut(const std::wstring& path, const std::wstring& name) const {
            writeOut(std::filesystem::path(path), name);
        }

        /// Writes the file to the physical filesystem into the given path
        void writeOut(const std::wstring& path) const {
            writeOut(path, getName());
        }

        /// Writes the file to the physical filesystem into the given path
        void writeOut(const std::filesystem::path& path) const {
            writeOut(path, getName());
        }

        /// Writes the file to the physical filesystem to the current dir
        void writeOut() const {
            writeOut(std::filesystem::current_path(), getName());
        }

        /// Writes the file and parent directories to the physical filesystem in the current directory
        void writeOutWithDirs() const {
            writeOut(std::filesystem::current_path() / std::filesystem::path(getPath().substr(1)).parent_path(), getName());
        }

        /// Writes the file and parent directories to the physical filesystem into the given path
        void writeOutWithDirs(const std::wstring &path) const {
            writeOut(path / std::filesystem::path(getPath().substr(1)).parent_path(), getName());
        }

        /// Writes the file and parent directories to the physical filesystem into the given path
        void writeOutWithDirs(const std::filesystem::path& path) const {
            writeOut(path / std::filesystem::path(getPath().substr(1)).parent_path(), getName());
        }

        /// Writes the file and parent directories to the physical filesystem into the given path with the given filename
        void writeOutWithDirs(const std::wstring &path, const std::wstring &name) const {
            writeOut(path / std::filesystem::path(getPath().substr(1)).parent_path(), name);
        }

        /// Writes the file and parent directories to the physical filesystem into the given path with the given filename
        void writeOutWithDirs(std::filesystem::path path, const std::wstring &name) const {
            writeOut(path / std::filesystem::path(getPath().substr(1)).parent_path(), name);
        }

        /// Gets if the FSObject is a file or not
        ///
        /// Because this is a File, it will always return true
        [[nodiscard]] bool isFile() const override { return true; }

        /// Gets the file's data
        [[nodiscard]] const std::vector<uint8_t>& getData() const { return data; }
        /// Gets the file's size (equiv. getData().size())
        [[nodiscard]] size_t getSize() const { return data.size(); }

        /// Output file data
        friend std::ostream& operator<<(std::ostream& os, const File &f) {
            os.write(reinterpret_cast<const char*>(f.data.data()), f.data.size());
            return os;
        }

        /// Output file data
        friend std::ostream& operator<<(std::ostream& os, const File *f) {
            os.write(reinterpret_cast<const char*>(f->data.data()), f->data.size());
            return os;
        }

        /// Output file info as string
        friend std::wostream& operator<<(std::wostream& wos, const File &f) {
            wos << L"File [name=" << f.getName() << L", size=" << f.getSize() << ", creationTime=" << f.getCreationTimestamp() << ", modifiedTime=" << f.getModifiedTimestamp() << L"]";
            return wos;
        }

        /// Output file info as string
        friend std::wostream& operator<<(std::wostream& wos, const File *f) {
            wos << L"File [name=" << f->getName() << L", size=" << f->getSize() << ", creationTime=" << f->getCreationTimestamp() << ", modifiedTime=" << f->getModifiedTimestamp() << L"]";
            return wos;
        }

        /// Push a byte into the file
        File& operator<<(const uint8_t b) {
            data.push_back(b);
            return *this;
        }

        /// Read/Write a byte from the file
        uint8_t& operator[](const int i) {
            return data[i];
        }

        /// Read a byte from the file
        uint8_t operator[](const int i) const {
            return data[i];
        }
    private:
        std::vector<uint8_t> data;
    };
} // namespace lce::fs


#endif
