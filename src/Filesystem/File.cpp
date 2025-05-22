//
// Created by Boreal on 5/20/2025.
//

#include "File.h"

#include <locale> 
#include <codecvt> 
#include <string> 

namespace lce::fs {
	
	// TODO: Move to BinaryIO and find a way that isn't deprecated
    
    std::string File::getName() const {
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert; // DEPRECATED
		return convert.to_bytes(name);     
	}
	
	void File::setName(std::string _name) {
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert; // DEPRECATED
		name = convert.from_bytes(_name);
	}
	
	std::vector<uint8_t> File::getDataVec() const {
        return std::vector<uint8_t>(this->data, this->data + this->size);
    }

    void File::setDataVec(std::vector<uint8_t> vector) {
        this->data = vector.data();
    }
    
    uint8_t* File::create() const {
		return data;
	}
}
