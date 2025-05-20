//
// Created by Boreal on 05/15/2025.
//

#include "Language.h"

namespace lce::loc {
	
	// "TODO: better way to do this (maybe not have separate function to pass io reference)"
	// I think its fiiine
	
	Language::Language(io::BinaryIO &io) {
		shouldReadByte = io.readBE<uint32_t>();
		
		if (shouldReadByte > 0) byte = io.readByte(); 
		
		code = io.readUtf8(io.readBE<uint16_t>());
		stringCount = io.readBE<uint32_t>();	
				
		for(int s = 0; s < stringCount; s++) {
			uint32_t stringSize = io.readBE<uint16_t>();
			strings.push_back(io.readUtf8(stringSize));
		}
	}
	
	uint32_t Language::getSize() const {
        uint32_t size = 0;
        
        size += sizeof(shouldReadByte); 
        if (shouldReadByte > 0) size += sizeof(byte);
        size += sizeof(uint16_t) + code.size();
        size += sizeof(stringCount);
        
        for (const auto& string : this->strings) {
			size += sizeof(uint16_t) + string.size();
        }

        return size;
    }
    
    uint8_t* Language::create() const {
		const uint32_t fileSize = this->getSize();
        uint8_t *data = new uint8_t[fileSize];
        io::BinaryIO io(data);
		
		io.writeBE<uint32_t>(shouldReadByte);
        if (shouldReadByte > 0) io.writeByte(byte);
			
		io.writeBE<uint16_t>(code.size());
		io.writeUtf8(code);
		
		io.writeBE<uint32_t>(stringCount);
		
		for (const auto& string : this->strings) {
			io.writeBE<uint16_t>(string.size());
			io.writeUtf8(string);
        }
        
        return io.getData();
	}
}
