//
// Created by Boreal on 05/15/2025.
//

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "LCE/io/Serializable.h"

#include "LCE/libLCE.h"
#include <BinaryIO/buffer/BinaryBuffer.h>

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace lce::loc {

    /** Used to hold strings and language metadata */
    class LIBLCE_API Language final : public io::Serializable {
      public:
        friend class LocalizationFile;

        /** Language ID found in LOC files */
        class Id final : public Serializable {
          public:
            explicit Id(const std::string &name)
                : m_name(name), m_id(std::hash<std::string>{}(name)) {};
            Id(const std::string &name, const uint32_t hash)
                : m_name(name), m_id(hash) {};

            bool operator==(const Id &other) const {
                return m_name == other.m_name && m_id == other.m_id;
            }

            uint32_t getId() const { return m_id; }

            const std::string &getName() const { return m_name; }

            std::uint8_t *serialize() const override {
                bio::buffer::BinaryBuffer io(this->getSize());

                io.writeString<char>(m_name, bio::util::ByteOrder::BIG, bio::util::string::StringLengthEncoding::LENGTH_PREFIX);
                io.writeBE<uint32_t>(m_id);

                return io.begin();
            };

            size_t getSize() const override {
                return sizeof(uint16_t) + m_name.size() + sizeof(uint32_t);
            };

          private:
            std::string m_name;
            uint32_t m_id;
        };

        explicit Language(bio::buffer::BinaryBuffer &io, std::vector<uint32_t> &keys);

        Language(const uint8_t _byte, const uint32_t _shouldReadByte,
                 std::string _code, std::vector<uint32_t> &keys)
            : m_unk(_byte), m_shouldReadByte(_shouldReadByte), m_keys(&keys),
              m_name(std::move(_code)) {
            for (auto &key : keys) {
                m_strings.emplace(key, "");
            }
        }

        Language(const uint8_t _byte, const uint32_t _shouldReadByte,
                 std::string _code)
            : m_unk(_byte), m_shouldReadByte(_shouldReadByte),
              m_keys(new std::vector<uint32_t>()), m_name(std::move(_code)) {}

        std::unordered_map<uint32_t, std::string> &getStrings();

        const std::string &getName() const;

        void setString(uint32_t id, const std::string &str);

        /** Nicer looking way to add data-driven strings
         *
         * Hashes the given ID, so you can call `setString("main.ok", "Ok");`
         *
         * @param id The ID you want to hash (e.g `main.ok`)
         * @param str The string the ID should correspond to
         *
         * @returns The hashed ID
         */
        uint32_t setString(const std::string &id, const std::string &str);

        std::string &getString(uint32_t id);

        /** Nicer looking way to get data-driven strings
         * This only works properly if you use the setString(const std::string
         * &id, const std::string &str) method
         *
         * Hashes the given ID, so you can call `getString("main.ok");` instead
         * of `getString(std::hash<std::string>{}("main.ok"));`
         *
         * @param id The ID of the string you want to get
         *
         * @returns The string ID
         */
        std::string &getString(const std::string &id);

        uint8_t getUnk() const;
        uint32_t getShouldReadByte() const;

        size_t getSize() const override;
        uint8_t *serialize() const override;

      protected:
        // protected because otherwise desync can happen
        // contemplating adding reference to key array in here

        uint32_t addString(const std::string &str);
        uint32_t addString(const std::string &str, const uint32_t &hash);

        void setName(const std::string &name) { this->m_name = name; }

      private:
        uint8_t m_unk;
        uint32_t m_shouldReadByte; // could be version?

        std::vector<uint32_t>
            *m_keys; // TODO: this makes the object not properly movable until
                    // this is changed...

        std::string m_name;
        std::unordered_map<uint32_t, std::string> m_strings;
    };
} // namespace lce::loc

// need this so that the id will hash properly
template <> struct std::hash<lce::loc::Language::Id> {
    size_t operator()(const lce::loc::Language::Id &k) const noexcept {
        return std::hash<uint32_t>{}(
            k.getId()); // we could also just return the ID itself can't we?
    }
}; // namespace std

#endif // LANGUAGE_H
