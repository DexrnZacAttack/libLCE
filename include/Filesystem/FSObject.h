//
// Created by DexrnZacAttack on 5/26/2025 (ZPC-I1)
//

#ifndef FSOBJECT_H
#define FSOBJECT_H
#include <utility>

#include "string"
#include <libLCE.h>

namespace lce::fs {
    class Directory;

    class LIBLCE_API FSObject {
        friend class Directory;
        friend class File;

    private:
        std::wstring name;
        std::uint64_t modifiedTime;
        std::uint64_t creationTime;
        Directory *parent;
    protected:
        explicit FSObject(std::wstring name) : name(std::move(name)) {
            const uint64_t ms = lce::system::getTimestamp();

            creationTime = ms;
            modifiedTime = ms;
            parent = nullptr;
        }

        /// Renames the object
        void setName(const std::wstring& name) {
            this->name = name;
            modifiedTime = lce::system::getTimestamp();
        }

        /// Set's the object's modified timestamp
        void setModifiedTimestamp(uint64_t n) {
            this->modifiedTime = n;
        }

        /// Set's the object's creation timestamp
        void setCreationTimestamp(uint64_t n) {
            this->creationTime = n;
        }
    public:
        virtual ~FSObject() = default;

        /// Gets if the object is a file, is overridden under File and Directory.
        [[nodiscard]] virtual bool isFile() const = 0;
        /// Gets the name of the object
        [[nodiscard]] std::wstring getName() const { return this->name; };
        /// Gets the modified timestamp of the object
        [[nodiscard]] std::uint64_t getModifiedTimestamp() const { return this->modifiedTime; };
        /// Gets the creation timestamp of the object
        [[nodiscard]] std::uint64_t getCreationTimestamp() const { return this->creationTime; };
        /// Gets the path of the object
        [[nodiscard]] std::wstring getPath() const;
    };

}

#endif //FSOBJECT_H
