//
// Created by Boreal on 05/15/2025.
//

// Documentation for this format is available at:
// https://team-lodestone.github.io/Documentation/LCE/File%20Types/LOC.html

#include "LCE/localization/LocalizationFile.h"
#include <BinaryIO/buffer/BinaryBuffer.h>
#include <algorithm>

#include <utility>

namespace lce::loc {

    LocalizationFile::LocalizationFile(
        const uint32_t version,
        std::unordered_map<Language::Id, Language> languages)
        : m_version(version), m_useUniqueIds(version == 2),
          m_languages(std::move(languages)) {}

    LocalizationFile::LocalizationFile(
        const uint32_t version, const std::vector<uint32_t> &keys,
        std::unordered_map<Language::Id, Language> languages)
        : m_version(version), m_useUniqueIds(true), m_keys(keys),
          m_languages(std::move(languages)) {}

    LocalizationFile::LocalizationFile(uint8_t *data) {
        bio::buffer::BinaryBuffer io(data);

        m_version = io.readBE<uint32_t>();
        const uint32_t lc = io.readBE<uint32_t>();

        if (m_version == 2) {
            this->m_useUniqueIds = io.readByte();
            const uint32_t c = io.readBE<uint32_t>();
            for (size_t i = 0; i < c; i++)
                this->m_keys.push_back(io.readBE<uint32_t>());
        }

        std::vector<Language::Id> ids;

        for (int i = 0; i < lc; i++) {
            const std::string code = io.readStringWithLength<char>(bio::util::ByteOrder::BIG, bio::util::string::StringLengthEncoding::LENGTH_PREFIX);
            const uint32_t id = io.readBE<uint32_t>();

            ids.push_back(Language::Id(code, id));
        }

        for (int i = 0; i < lc; i++) {
            Language l = Language(io, this->m_keys);
            this->m_languages.emplace(std::move(ids[i]), std::move(l));
        }
    }

    size_t LocalizationFile::getSize() const {
        uint32_t size = 0;

        size += sizeof(uint32_t); // version
        size += sizeof(uint32_t); // lang count

        if (m_version == 2) { // keys
            size += sizeof(bool);
            size += sizeof(uint32_t);
            size += this->m_keys.size() * sizeof(uint32_t);
        }

        for (const auto &[id, lang] : m_languages) {
            size += id.getSize();   // ids
            size += lang.getSize(); // languages
        }

        return size;
    }

    uint8_t *LocalizationFile::serialize() const {
        const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        bio::buffer::BinaryBuffer io(data);

        io.writeBE<uint32_t>(m_version);
        io.writeBE<uint32_t>(m_languages.size());

        if (m_version > 1) {
            io.writeByte(m_useUniqueIds);
            io.writeBE<uint32_t>(m_keys.size());
            for (const uint32_t k : m_keys) {
                io.writeBE<uint32_t>(k);
            }
        }

        for (const auto &[id, lang] : m_languages) {
            io.writeBytes(id.serialize(), id.getSize());
        }

        for (const auto &[id, lang] : m_languages) {
            io.writeBytes(lang.serialize(), lang.getSize());
        }

        return io.begin();
    }
    Language *LocalizationFile::createLanguage(const std::string &name,
                                               const uint8_t _byte,
                                               const uint32_t _shouldReadByte) {
        if (languageExists(name))
            throw std::runtime_error("Language already exists with name: " +
                                     name);

        Language::Id id = Language::Id(name);
        Language language = Language(_byte, _shouldReadByte, name, this->m_keys);
        this->m_languages.emplace(id, language);

        return getLanguage(name);
    }

    Language *LocalizationFile::getLanguage(const std::string &name) {
        for (auto &[id, lang] : m_languages) {
            if (id.getName() == name)
                return &lang;
        }

        return nullptr;
    }
    bool LocalizationFile::languageExists(const std::string &name) {
        for (auto &[id, lang] : m_languages) {
            if (id.getName() == name)
                return true;
        }

        return false;
    }

    uint32_t LocalizationFile::createString(const uint32_t id) {
        m_keys.reserve(m_keys.size() + 1);
        m_keys.push_back(id);

        for (auto &[i, lang] : m_languages)
            lang.addString("", id);

        return id;
    }

    uint32_t LocalizationFile::setString(const std::string &language,
                                         const std::string &id,
                                         const std::string &str) {
        const uint32_t h = std::hash<std::string>{}(id);
        setString(language, h, str);

        return h;
    }

    void LocalizationFile::setString(const std::string &language,
                                     const uint32_t id,
                                     const std::string &str) {
        Language *lang = getLanguage(language);
        if (!lang)
            throw std::runtime_error("Language not found: " + language);

        const auto n = std::find(m_keys.begin(), m_keys.end(), id);
        const uint32_t h = (n == m_keys.end()) ? createString(id) : *n;

        lang->setString(h, str);
    }

    std::pair<const Language::Id, Language> *
    LocalizationFile::getLanguageWithId(const std::string &name) {
        for (std::pair<const Language::Id, Language> &a : m_languages) {
            if (a.first.getName() == name)
                return &a;
        }

        return nullptr;
    }

    void LocalizationFile::renameLanguage(const std::string &name,
                                          const std::string &n) {
        const auto it = getLanguageWithId(name);

        // yoink
        const uint32_t i = it->first.getId();

        Language lang = std::move(it->second);
        m_languages.erase(it->first);

        // shove back in
        lang.setName(n);
        m_languages.emplace(Language::Id(n, i), std::move(lang));
    }

    std::string &LocalizationFile::getString(const std::string &language,
                                             const std::string &id) {
        Language *lang = getLanguage(language);
        if (!lang)
            throw std::runtime_error("Language not found: " + language);

        return lang->getString(id);
    }

    std::string &LocalizationFile::getString(const std::string &language,
                                             const uint32_t id) {
        Language *lang = getLanguage(language);
        if (!lang)
            throw std::runtime_error("Language not found: " + language);

        return lang->getString(id);
    }

    std::unordered_map<Language::Id, Language> &
    LocalizationFile::getLanguages() {
        return this->m_languages;
    }

    uint32_t LocalizationFile::getVersion() const { return this->m_version; }
} // namespace lce::loc
