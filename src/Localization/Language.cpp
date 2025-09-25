//
// Created by Boreal on 05/15/2025.
//

#include <Localization/Language.h>

namespace lce::loc {

    Language::Language(io::BinaryIO &io, std::vector<uint32_t> &keys)
        : mKeys(&keys) {
        // could be a version?
        this->mShouldReadByte = io.readBE<uint32_t>();

        this->mUnk = 0;
        if (mShouldReadByte > 0)
            this->mUnk = io.readByte();

        this->mName = io.readString(io.readBE<uint16_t>());

        const uint32_t c = io.readBE<uint32_t>();

        if (mKeys->size() < c)
            mKeys->reserve(c);

        mStrings.reserve(c);
        for (int s = 0; s < c; s++) {
            if (s >= mKeys->size()) {
                mKeys->push_back(s);
            }

            const uint32_t ss = io.readBE<uint16_t>();
            mStrings.emplace(keys[s], io.readString(ss));
        }
    }

    size_t Language::getSize() const {
        uint32_t size = 0;

        size += sizeof(mShouldReadByte);
        if (mShouldReadByte > 0)
            size += sizeof(mUnk);
        size += sizeof(uint16_t) + mName.size();
        size += sizeof(uint32_t);

        for (const auto &[id, str] : this->mStrings) {
            size += sizeof(uint16_t) + str.size();
        }

        return size;
    }

    uint8_t *Language::serialize() const {
        const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        io::BinaryIO io(data);

        io.writeBE<uint32_t>(mShouldReadByte);
        if (mShouldReadByte > 0)
            io.writeByte(mUnk);

        io.writeBE<uint16_t>(mName.size());
        io.writeString(mName);

        io.writeBE<uint32_t>(mStrings.size());
        for (const uint32_t k : *this->mKeys) {
            const auto &s = this->mStrings.find(k);

            io.writeBE<uint16_t>(s->second.size());
            io.writeString(s->second);
        }

        return io.getData();
    }

    uint32_t Language::addString(const std::string &str) {
        return addString(str, std::hash<std::string>{}(str));
    }

    std::unordered_map<uint32_t, std::string> &Language::getStrings() {
        return this->mStrings;
    }

    const std::string &Language::getName() const { return this->mName; }

    void Language::setString(const uint32_t id, const std::string &str) {
        if (!mStrings.count(id))
            throw std::runtime_error("String has does not exist in map");

        this->mStrings[id] = std::move(str);
    }

    uint32_t Language::setString(const std::string &id,
                                 const std::string &str) {
        const uint32_t h = std::hash<std::string>{}(id);
        setString(h, str);

        return h;
    }

    std::string &Language::getString(const uint32_t id) { return mStrings[id]; }

    std::string &Language::getString(const std::string &id) {
        return getString(std::hash<std::string>{}(id));
    }

    uint32_t Language::addString(const std::string &str, const uint32_t &hash) {
        if (mStrings.count(hash))
            throw std::runtime_error("String hash already exists in map");

        mStrings[hash] = std::move(str);

        return hash;
    }

    uint8_t Language::getUnk() const { return this->mUnk; }

    uint32_t Language::getShouldReadByte() const {
        return this->mShouldReadByte;
    }
} // namespace lce::loc
