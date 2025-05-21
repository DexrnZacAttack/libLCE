//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "../IO/BinaryIO.h"
#include "InnerFile.h"

#include <locale> 
#include <codecvt> 
#include <string> 

namespace lce::file {
	
	// TODO: Move to BinaryIO and find a way that isn't deprecated
    
    std::string InnerFile::getName() const {
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
		return convert.to_bytes(name);     
	}
	
	void InnerFile::setName(std::string _name) {
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert; 
		name = convert.from_bytes(_name);
	}
	
	std::vector<uint8_t> InnerFile::getDataVec() const {
        return std::vector<uint8_t>(this->data, this->data + this->size);
    }

    void InnerFile::setDataVec(std::vector<uint8_t> vector) {
        this->data = vector.data();
    }
    
    uint8_t* InnerFile::create() const {
		return data;
	}
}
