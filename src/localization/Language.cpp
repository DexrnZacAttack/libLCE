//
// Created by Boreal on 05/15/2025.
//

#include "LCE/localization/Language.h"

namespace lce::loc {

    Language::Language(bio::buffer::BinaryBuffer &io, std::vector<uint32_t> &keys)
        : m_keys(&keys) {
        // could be a version?
        this->m_shouldReadByte = io.readBE<uint32_t>();

        this->m_unk = 0;
        if (m_shouldReadByte > 0)
            this->m_unk = io.readByte();

        this->m_name = io.readStringWithLength<char>(bio::util::ByteOrder::BIG, bio::util::string::StringLengthEncoding::NULL_TERMINATE);

        const uint32_t c = io.readBE<uint32_t>();

        if (m_keys->size() < c)
            m_keys->reserve(c);

        m_strings.reserve(c);
        for (int s = 0; s < c; s++) {
            if (s >= m_keys->size()) {
                m_keys->push_back(s);
            }

            m_strings.emplace(keys[s], io.readStringWithLength<char>(bio::util::ByteOrder::BIG, bio::util::string::StringLengthEncoding::NULL_TERMINATE));
        }
    }

    size_t Language::getSize() const {
        uint32_t size = 0;

        size += sizeof(m_shouldReadByte);
        if (m_shouldReadByte > 0)
            size += sizeof(m_unk);
        size += sizeof(uint16_t) + m_name.size();
        size += sizeof(uint32_t);

        for (const auto &[id, str] : this->m_strings) {
            size += sizeof(uint16_t) + str.size();
        }

        return size;
    }

    uint8_t *Language::serialize() const {
        const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        bio::buffer::BinaryBuffer io(data);

        io.writeBE<uint32_t>(m_shouldReadByte);
        if (m_shouldReadByte > 0)
            io.writeByte(m_unk);

        io.writeString<char>(m_name, bio::util::ByteOrder::BIG, bio::util::string::StringLengthEncoding::LENGTH_PREFIX);

        io.writeBE<uint32_t>(m_strings.size());
        for (const uint32_t k : *this->m_keys) {
            const auto &s = this->m_strings.find(k);

            io.writeString<char>(s->second, bio::util::ByteOrder::BIG, bio::util::string::StringLengthEncoding::LENGTH_PREFIX);
        }

        return io.begin();
    }

    uint32_t Language::addString(const std::string &str) {
        return addString(str, std::hash<std::string>{}(str));
    }

    std::unordered_map<uint32_t, std::string> &Language::getStrings() {
        return this->m_strings;
    }

    const std::string &Language::getName() const { return this->m_name; }

    void Language::setString(const uint32_t id, const std::string &str) {
        if (!m_strings.count(id))
            throw std::runtime_error("String has does not exist in map");

        this->m_strings[id] = std::move(str);
    }

    uint32_t Language::setString(const std::string &id,
                                 const std::string &str) {
        const uint32_t h = std::hash<std::string>{}(id);
        setString(h, str);

        return h;
    }

    std::string &Language::getString(const uint32_t id) { return m_strings[id]; }

    std::string &Language::getString(const std::string &id) {
        return getString(std::hash<std::string>{}(id));
    }

    uint32_t Language::addString(const std::string &str, const uint32_t &hash) {
        if (m_strings.count(hash))
            throw std::runtime_error("String hash already exists in map");

        m_strings[hash] = std::move(str);

        return hash;
    }

    uint8_t Language::getUnk() const { return this->m_unk; }

    uint32_t Language::getShouldReadByte() const {
        return this->m_shouldReadByte;
    }
} // namespace lce::loc
