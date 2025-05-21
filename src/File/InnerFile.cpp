//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "../IO/BinaryIO.h"
#include "InnerFile.h"

namespace lce::file {
    
    uint8_t* InnerFile::create() const {
		return data;
	}
	
	void InnerFile::setData(uint64_t _size, uint64_t _offset, uint8_t* _data) {
		size = _size;
		offset = _offset;
		data = _data;
	}
}
