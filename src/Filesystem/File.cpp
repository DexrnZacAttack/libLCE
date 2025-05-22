//
// Created by Boreal on 5/20/2025.
//

#include "File.h"
#include <string>
#include "../IO/BinaryIO.h" 

namespace lce::fs {
	
	// TODO: Move to BinaryIO and find a way that isn't deprecated
    
    std::string File::getName() const {
		return io::BinaryIO::u16stringToString(name);
	}
	
	void File::setName(std::string _name) {
		this->name = io::BinaryIO::stringToU16String(_name);
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
