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

// TODO: finish API
namespace lce::loc {
    // WARNING!!!!: I believe this code is so cursed that it scared off some
    // people that were trying to help
    //
    // Not even I fully understand it...

    /** Used to hold strings and language metadata */
    class LIBLCE_API Language final : public io::Serializable {
      public:
        friend class LocalizationFile;

        /** Language ID found in LOC files */
        class Id final : public Serializable {
          public:
            explicit Id(const std::string &name)
                : mName(name), mId(std::hash<std::string>{}(name)) {};
            Id(const std::string &name, const uint32_t hash)
                : mName(name), mId(hash) {};

            bool operator==(const Id &other) const {
                return mName == other.mName && mId == other.mId;
            }

            uint32_t getId() const { return mId; }

            const std::string &getName() const { return mName; }

            std::uint8_t *serialize() const override {
                bio::buffer::BinaryBuffer io(this->getSize());

                io.writeBE<uint16_t>(mName.size());
                io.writeString(mName, false);
                io.writeBE<uint32_t>(mId);

                return io.begin();
            };

            size_t getSize() const override {
                return sizeof(uint16_t) + mName.size() + sizeof(uint32_t);
            };

          private:
            std::string mName;
            uint32_t mId;
        };

        explicit Language(bio::buffer::BinaryBuffer &io, std::vector<uint32_t> &keys);

        Language(const uint8_t _byte, const uint32_t _shouldReadByte,
                 std::string _code, std::vector<uint32_t> &keys)
            : mUnk(_byte), mShouldReadByte(_shouldReadByte), mKeys(&keys),
              mName(std::move(_code)) {
            for (auto &key : keys) {
                mStrings.emplace(key, "");
            }
        }

        Language(const uint8_t _byte, const uint32_t _shouldReadByte,
                 std::string _code)
            : mUnk(_byte), mShouldReadByte(_shouldReadByte),
              mKeys(new std::vector<uint32_t>()), mName(std::move(_code)) {}

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

        void setName(const std::string &name) { this->mName = name; }

      private:
        uint8_t mUnk;
        uint32_t mShouldReadByte; // could be version?

        std::vector<uint32_t>
            *mKeys; // TODO: this makes the object not properly movable until
                    // this is changed...

        std::string mName;
        std::unordered_map<uint32_t, std::string> mStrings;
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
