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

    class LIBLCE_API LocalizationFile : public io::Serializable {
      public:
        LocalizationFile(uint8_t *data);
        LocalizationFile();
        LocalizationFile(uint32_t version, uint32_t languageCount,
                         std::unordered_map<uint32_t, std::string> langIds,
                         std::vector<Language> languages);
        LocalizationFile(uint32_t version, uint32_t languageCount,
                         std::vector<uint32_t> keys,
                         std::unordered_map<uint32_t, std::string> langIds,
                         std::vector<Language> languages);

        size_t getSize() const override;
        uint8_t *serialize() const override;

        uint32_t version;
        uint32_t languageCount;

        bool useUniqueIds;
        uint32_t keyCount;

        std::vector<uint32_t> keys; // should be in with the strings themselves
                                    // in unordered_map, see Language.h
        std::unordered_map<uint32_t, std::string> langIds;
        std::vector<Language> languages;
    };
} // namespace lce::loc

#endif // LOCALIZATIONFILE_H
