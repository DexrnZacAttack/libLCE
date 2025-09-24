//
// Created by DexrnZacAttack on 5/26/2025 (ZPC-I1)
//

#ifndef FSOBJECT_H
#define FSOBJECT_H
#include <utility>

#include <libLCE.h>
#include <sstream>
#include <string>

namespace lce::fs {
    class Directory;

    /** A base Filesystem Object
     *
     * @see File
     * @see Directory
     */
    class LIBLCE_API FSObject {
        friend class Filesystem;
        friend class Directory;
        friend class File;

      protected:
        explicit FSObject(std::wstring name) : name(std::move(name)) {
            const uint64_t ms = system::getTimestamp();

            creationTime = ms;
            modifiedTime = ms;
            parent = nullptr;
        }

        explicit FSObject(std::wstring name, uint64_t creationTime,
                          uint64_t modifiedTime, Directory *parent)
            : name(std::move(name)) {
            this->creationTime = creationTime;
            this->modifiedTime = modifiedTime;
            this->parent = parent; // TODO: should this be allowed?
        }

        /** Renames the object
         *
         * @param name The new name you'd like to give to the object
         */
        void setName(const std::wstring &name) {
            this->name = name;
            modifiedTime = lce::system::getTimestamp();
        }

        /** Set's the object's modified timestamp
         *
         * @param n The new modification timestamp you'd like to give to the
         * object
         */
        void setModifiedTimestamp(const uint64_t n) { this->modifiedTime = n; }

        /** Set's the object's creation timestamp
         *
         * @param n The new creation timestamp you'd like to give to the object
         */
        void setCreationTimestamp(const uint64_t n) { this->creationTime = n; }

        friend std::wostream &operator<<(std::wostream &wos,
                                         const FSObject &f) {
            wos << f.toString();
            return wos;
        }

        friend std::wostream &operator<<(std::wostream &wos,
                                         const FSObject *f) {
            wos << f->toString();
            return wos;
        }

      public:
        FSObject(FSObject &&) = default;
        FSObject &operator=(FSObject &&) = default;

        [[nodiscard]] bool hasParent() const { return this->parent != nullptr; }

        /** Returns a string containing info about the object */
        [[nodiscard]] virtual std::wstring toString() const {
            return L"FSObject[name=" + this->getName() + L", creationTime=" +
                   std::to_wstring(this->getCreationTimestamp()) +
                   L", modifiedTime=" +
                   std::to_wstring(this->getModifiedTimestamp()) + L"]";
        }

        /// Deletes/Removes an object from its parent
        virtual void remove();

        /** Gets the size of the object */
        virtual size_t getSize() const = 0;

        virtual ~FSObject() = default;

        /** @returns \c true if the object is a file.
         *
         * @see File::isFile
         * @see Directory::isFile
         */
        [[nodiscard]] virtual bool isFile() const = 0;
        /** @returns The object's name */
        [[nodiscard]] std::wstring getName() const { return this->name; };
        /** @returns The object's last modified timestamp */
        [[nodiscard]] std::uint64_t getModifiedTimestamp() const {
            return this->modifiedTime;
        };
        /** @returns The object's creation timestamp */
        [[nodiscard]] std::uint64_t getCreationTimestamp() const {
            return this->creationTime;
        };
        /** @returns The object's full path */
        [[nodiscard]] std::wstring getPath() const;

        [[nodiscard]] Directory *getParent() const { return this->parent; };

      private:
        std::wstring name;
        std::uint64_t modifiedTime;
        std::uint64_t creationTime;
        Directory *parent;
    };

} // namespace lce::fs

#endif // FSOBJECT_H
