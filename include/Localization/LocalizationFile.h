//
// Created by Boreal on 05/15/2025.
//

#ifndef LOCALIZATIONFILE_H
#define LOCALIZATIONFILE_H

#include "IO/Serializable.h"

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Localization/Language.h>
#include <libLCE.h>

namespace lce::loc {

    /** LCE Localization (.LOC) File
     *
     * \brief Used for storing strings for supported languages.
     */
    class LIBLCE_API LocalizationFile final : public io::Serializable {
      public:
        explicit LocalizationFile(uint8_t *data);
        LocalizationFile() = default;
        LocalizationFile(uint32_t version,
                         std::unordered_map<Language::Id, Language> languages);
        LocalizationFile(uint32_t version, const std::vector<uint32_t> &keys,
                         std::unordered_map<Language::Id, Language> languages);

        size_t getSize() const override;
        uint8_t *serialize() const override;

        Language *createLanguage(const std::string &name,
                                 const uint8_t _byte = 2,
                                 const uint32_t _shouldReadByte = 1);

        Language *getLanguage(const std::string &name);

        bool languageExists(const std::string &name);

        uint32_t createString(uint32_t id);

        /** Nicer looking way to add data-driven strings
         *
         * Hashes the given ID, so you can call `setString("main.ok", "Ok");`
         *
         * @param language The ID of the language you want to add the string to
         * @param id The ID you want to hash (e.g `main.ok`)
         * @param str The string the ID should correspond to
         *
         * @returns The hashed ID
         */
        uint32_t setString(const std::string &language, const std::string &id,
                           const std::string &str);

        void setString(const std::string &language, uint32_t id,
                       const std::string &str);

        std::pair<const Language::Id, Language> *
        getLanguageWithId(const std::string &name);

        void renameLanguage(const std::string &name, const std::string &n);

        size_t getLanguageCount() const { return this->mLanguages.size(); };

        size_t getStringCount() const { return this->mKeys.size(); };

        /** Nicer looking way to get data-driven strings
         * This only works properly if you use the setString(const std::string
         * &id, const std::string &str) method
         *
         * Hashes the given ID, so you can call `getString("main.ok");` instead
         * of `getString(std::hash<std::string>{}("main.ok"));`
         *
         * @param language The ID of the language you want to get the string
         * from
         * @param id The ID of the string you want to get
         *
         * @returns The string
         *
         * @throws std::runtime_error If the language does not exist
         */
        std::string &getString(const std::string &language,
                               const std::string &id);

        std::string &getString(const std::string &language, uint32_t id);

        std::unordered_map<Language::Id, Language> &getLanguages();

        uint32_t getVersion() const;

      private:
        uint32_t mVersion;

        bool mUseUIDs;
        std::vector<uint32_t> mKeys;

        std::unordered_map<Language::Id, Language> mLanguages;
    };
} // namespace lce::loc

#endif // LOCALIZATIONFILE_H
