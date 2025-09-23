//
// Created by DexrnZacAttack on 9/22/25 using zPc-i2.
//
#pragma once
#include <cstdint>
#include <filesystem>

namespace lce::io {
    /** Used for classes that can be serialized and written out to a filesystem
     */
    class Serializable {
      public:
        virtual ~Serializable() = default;

        /** Serializes the object
         *
         * @see getSize() - Used to get the size of the serialized data
         * @returns The serialized object
         */
        virtual std::uint8_t *serialize() const = 0;

        /** Gets the size of the serialized data */
        virtual size_t getSize() const = 0;

        /** Writes out the file to a physical path on the filesystem */
        virtual void writeFile(const std::filesystem::path &p) const;
    };
} // namespace lce::io