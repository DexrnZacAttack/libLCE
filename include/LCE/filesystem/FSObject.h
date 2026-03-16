//
// Created by DexrnZacAttack on 5/26/2025 (ZPC-I1)
//

#ifndef FSOBJECT_H
#define FSOBJECT_H
#include <utility>

#include "LCE/libLCE.h"
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

    public:
        using char_t = char;
        using string_t = std::basic_string<char_t>;
        using ostringstream_t = std::basic_ostringstream<char_t>;

        using path_t = string_t;
        using name_t = string_t;

      protected:
        explicit FSObject(FSObject::name_t name) : m_name(std::move(name)) {
            const uint64_t ms = system::getTimestamp();

            m_creationTime = ms;
            m_modifiedTime = ms;
            m_parent = nullptr;
        }

        explicit FSObject(FSObject::name_t name, const uint64_t creationTime,
                          const uint64_t modifiedTime, Directory *parent)
            : m_name(std::move(name)) {
            this->m_creationTime = creationTime;
            this->m_modifiedTime = modifiedTime;
            this->m_parent = parent; // TODO: should this be allowed?
        }

        /** Renames the object
         *
         * @param name The new name you'd like to give to the object
         */
        void setName(const FSObject::name_t &name) {
            this->m_name = name;
            m_modifiedTime = lce::system::getTimestamp();
        }

        /** Set's the object's modified timestamp
         *
         * @param n The new modification timestamp you'd like to give to the
         * object
         */
        void setModifiedTimestamp(const uint64_t n) { this->m_modifiedTime = n; }

        /** Set's the object's creation timestamp
         *
         * @param n The new creation timestamp you'd like to give to the object
         */
        void setCreationTimestamp(const uint64_t n) { this->m_creationTime = n; }

        friend ostringstream_t &operator<<(ostringstream_t &wos,
                                         const FSObject &f) {
            wos << f.toString();
            return wos;
        }

        friend ostringstream_t &operator<<(ostringstream_t &wos,
                                         const FSObject *f) {
            wos << f->toString();
            return wos;
        }

      public:
        FSObject(FSObject &&) = default;
        FSObject &operator=(FSObject &&) = default;

        [[nodiscard]] bool hasParent() const {
            return this->m_parent != nullptr;
        }

        /** Returns a string containing info about the object */
        [[nodiscard]] virtual FSObject::string_t toString() const {
            return "FSObject[name=" + this->getName() + ", creationTime=" +
                   std::to_string(this->getCreationTimestamp()) +
                   ", modifiedTime=" +
                   std::to_string(this->getModifiedTimestamp()) + "]";
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
        [[nodiscard]] FSObject::name_t getName() const { return this->m_name; }
        /** @returns The object's last modified timestamp */
        [[nodiscard]] std::uint64_t getModifiedTimestamp() const {
            return this->m_modifiedTime;
        }
        /** @returns The object's creation timestamp */
        [[nodiscard]] std::uint64_t getCreationTimestamp() const {
            return this->m_creationTime;
        }
        /** @returns The object's full path */
        [[nodiscard]] FSObject::path_t getPath() const;

        [[nodiscard]] Directory *getParent() const { return this->m_parent; }

      private:
        FSObject::name_t m_name;
        std::uint64_t m_modifiedTime;
        std::uint64_t m_creationTime;
        Directory *m_parent;
    };

} // namespace lce::fs

#endif // FSOBJECT_H
