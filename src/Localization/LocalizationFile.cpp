//
// Created by Boreal on 05/15/2025.
//

// Documentation for this format is available at:
// https://team-lodestone.github.io/Documentation/LCE/File%20Types/LOC.html

#include <IO/BinaryIO.h>
#include <Localization/LocalizationFile.h>
#include <algorithm>

#include <utility>

namespace lce::loc {

    LocalizationFile::LocalizationFile(
        const uint32_t version,
        std::unordered_map<Language::Id, Language> languages)
        : mVersion(version), mUseUIDs(version == 2),
          mLanguages(std::move(languages)) {}

    LocalizationFile::LocalizationFile(
        const uint32_t version, const std::vector<uint32_t> &keys,
        std::unordered_map<Language::Id, Language> languages)
        : mVersion(version), mUseUIDs(true), mKeys(keys),
          mLanguages(std::move(languages)) {}

    LocalizationFile::LocalizationFile(uint8_t *data) {
        io::BinaryIO io(data);

        mVersion = io.readBE<uint32_t>();
        const uint32_t lc = io.readBE<uint32_t>();

        if (mVersion == 2) {
            this->mUseUIDs = io.readByte();
            const uint32_t c = io.readBE<uint32_t>();
            for (size_t i = 0; i < c; i++)
                this->mKeys.push_back(io.readBE<uint32_t>());
        }

        std::vector<Language::Id> ids;

        for (int i = 0; i < lc; i++) {
            const std::string code = io.readString(io.readBE<uint16_t>());
            const uint32_t id = io.readBE<uint32_t>();

            ids.push_back(Language::Id(code, id));
        }

        for (int i = 0; i < lc; i++) {
            Language l = Language(io, this->mKeys);
            this->mLanguages.emplace(std::move(ids[i]), std::move(l));
        }
    }

    size_t LocalizationFile::getSize() const {
        uint32_t size = 0;

        size += sizeof(uint32_t); // version
        size += sizeof(uint32_t); // lang count

        if (mVersion == 2) { // keys
            size += sizeof(bool);
            size += sizeof(uint32_t);
            size += this->mKeys.size() * sizeof(uint32_t);
        }

        for (const auto &[id, lang] : mLanguages) {
            size += id.getSize();   // ids
            size += lang.getSize(); // languages
        }

        return size;
    }

    uint8_t *LocalizationFile::serialize() const {
        const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        io::BinaryIO io(data);

        io.writeBE<uint32_t>(mVersion);
        io.writeBE<uint32_t>(mLanguages.size());

        if (mVersion == 2) {
            io.writeByte(mUseUIDs);
            io.writeBE<uint32_t>(mKeys.size());
            for (const uint32_t k : mKeys) {
                io.writeBE<uint32_t>(k);
            }
        }

        for (const auto &[id, lang] : mLanguages) {
            io.writeBytes(id.serialize(), id.getSize());
        }

        for (const auto &[id, lang] : mLanguages) {
            io.writeBytes(lang.serialize(), lang.getSize());
        }

        return io.getData();
    }
    Language *LocalizationFile::createLanguage(const std::string &name,
                                               const uint8_t _byte,
                                               const uint32_t _shouldReadByte) {
        if (languageExists(name))
            throw std::runtime_error("Language already exists with name: " +
                                     name);

        Language::Id id = Language::Id(name);
        Language language = Language(_byte, _shouldReadByte, name, this->mKeys);
        this->mLanguages.emplace(id, language);

        return getLanguage(name);
    }

    Language *LocalizationFile::getLanguage(const std::string &name) {
        for (auto &[id, lang] : mLanguages) {
            if (id.getName() == name)
                return &lang;
        }

        return nullptr;
    }
    bool LocalizationFile::languageExists(const std::string &name) {
        for (auto &[id, lang] : mLanguages) {
            if (id.getName() == name)
                return true;
        }

        return false;
    }

    uint32_t LocalizationFile::createString(const uint32_t id) {
        mKeys.reserve(mKeys.size() + 1);
        mKeys.push_back(id);

        for (auto &[i, lang] : mLanguages)
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

        const auto n = std::find(mKeys.begin(), mKeys.end(), id);
        const uint32_t h = (n == mKeys.end()) ? createString(id) : *n;

        lang->setString(h, str);
    }

    std::pair<const Language::Id, Language> *
    LocalizationFile::getLanguageWithId(const std::string &name) {
        for (std::pair<const Language::Id, Language> &a : mLanguages) {
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
        mLanguages.erase(it->first);

        // shove back in
        lang.setName(n);
        mLanguages.emplace(Language::Id(n, i), std::move(lang));
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
        return this->mLanguages;
    }

    uint32_t LocalizationFile::getVersion() const { return this->mVersion; }
} // namespace lce::loc
