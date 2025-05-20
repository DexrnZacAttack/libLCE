//
// Created by Boreal on 05/15/2025.
//

#include "LocalizationFile.h"
#include "../IO/BinaryIO.h"

namespace lce::loc {
	
	LocalizationFile::LocalizationFile(uint32_t version, uint32_t languageCount, 
		std::unordered_map<uint32_t, std::string> langIds, std::vector<Language> languages)
		: version(version), 
		languageCount(languageCount), 
		langIds(langIds), 
		languages(languages) {}
	
	LocalizationFile::LocalizationFile(uint32_t version, uint32_t languageCount, std::vector<uint32_t> keys, 
		std::unordered_map<uint32_t, std::string> langIds, std::vector<Language> languages)
		: version(version), 
		useUniqueIds(true),
		keyCount(keys.size()),
		keys(keys),
		languageCount(languageCount), 
		langIds(langIds), 
		languages(languages) {}
	
	LocalizationFile::LocalizationFile() = default;
	
	LocalizationFile::LocalizationFile(uint8_t *data) {
		io::BinaryIO io(data);
		
		version = io.readBE<uint32_t>();
		languageCount = io.readBE<uint32_t>();
		
		if (version == 2) {
			useUniqueIds = io.readByte();
			keyCount = io.readBE<uint32_t>();
			for (size_t i = 0; i < keyCount; i++)
				keys.push_back(io.readBE<uint32_t>());
		}
		
		for (int i = 0; i < languageCount; i++) {
			// These need to execute in this order
			std::string code = io.readUtf8(io.readBE<uint16_t>());
			uint32_t id = io.readBE<uint32_t>();
			
			langIds[id] = code;
		}

		for (int i = 0; i < languageCount; i++) {
			languages.push_back( Language(io) );
		}
	}
	
	uint32_t LocalizationFile::getSize() const {
		uint32_t size = 0;
		
		size += sizeof(version);
		size += sizeof(languageCount);
		
		if(version == 2) {
			size += sizeof(useUniqueIds);
			size += sizeof(keyCount);
		}
		
		for(const auto& langId : langIds) {
			size += langId.second.size() + ( sizeof(uint16_t) + sizeof(langId.first) );
		}
		
		for(const auto& language : languages) {
			size += language.getSize();
		}

		return size;
	}
	
	uint8_t* LocalizationFile::create() const {
		const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        io::BinaryIO io(data);
		
		io.writeBE<uint32_t>(version);
		io.writeBE<uint32_t>(languageCount);
		
		if(version == 2) {
			io.writeByte(useUniqueIds);
			io.writeBE<uint32_t>(keyCount);
		}
		
		for(const auto& langId : langIds) {
			io.writeBE<uint16_t>(langId.second.size());
			io.writeUtf8(langId.second);
			io.writeBE<uint32_t>(langId.first);
		}
		
		for(const auto& language : languages) {
			io.writeBytes(language.create(), language.getSize());
		}
		
        return io.getData();
	}
}
