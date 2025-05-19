//
// Created by Boreal on 05/15/2025.
//

#ifndef LOCALIZATIONFILE_H
#define LOCALIZATIONFILE_H
#include "../libLCE.h"

#include <utility>
#include <string>
#include <vector>
#include <map>

#include "Language.h"

namespace lce::loc {
	using LangId = std::pair<std::string, uint32_t>;
	
	// TODO: Make these structs into classes
	
	class LIBLCE_API LocalizationFile {
	public:		
		LocalizationFile(uint8_t* data);
		
		uint32_t getSize() const;
		uint8_t* create() const;
	
		uint32_t version;
		uint32_t languageCount;
		
		bool useUniqueIds;
		uint32_t keyCount;
		
		std::vector<uint32_t> keys;
		std::vector<LangId> langIds;
		std::vector<Language> languages;
	};
}

#endif //LOCALIZATIONFILE_H
