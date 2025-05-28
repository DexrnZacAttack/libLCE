//
// Created by Boreal on 05/15/2025.
//

#ifndef LOCALIZATIONFILE_H
#define LOCALIZATIONFILE_H

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

#include "../libLCE.h"
#include "Language.h"

namespace lce::loc {

	class LIBLCE_API LocalizationFile {
	public:		
		LocalizationFile(uint8_t* data);
		LocalizationFile();
		LocalizationFile(uint32_t version, uint32_t languageCount, 
			std::unordered_map<uint32_t, std::string> langIds, std::vector<Language> languages);
		LocalizationFile(uint32_t version, uint32_t languageCount, std::vector<uint32_t> keys,
			std::unordered_map<uint32_t, std::string> langIds, std::vector<Language> languages);
		
		uint64_t getSize() const;
		uint8_t* toData() const;
	
		uint32_t version;
		uint32_t languageCount;
		
		bool useUniqueIds;
		uint32_t keyCount;
		
		std::vector<uint32_t> keys;
		std::unordered_map<uint32_t, std::string> langIds;
		std::vector<Language> languages;
	};
}

#endif //LOCALIZATIONFILE_H
