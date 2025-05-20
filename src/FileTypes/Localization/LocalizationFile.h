//
// Created by Boreal on 05/15/2025.
//

#ifndef LOCALIZATIONFILE_H
#define LOCALIZATIONFILE_H
#include "../../libLCE.h"

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

#include "../File.h"
#include "Language.h"

namespace lce::loc {
	
	class LIBLCE_API LocalizationFile : File {
	public:		
		LocalizationFile(uint8_t* data);
		
		uint32_t getSize() const override;
		uint8_t* create() const override;
	
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
