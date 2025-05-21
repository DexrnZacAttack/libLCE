//
// Created by DexrnZacAttack on 12/19/2024.
//

#include "../IO/BinaryIO.h"
#include "InnerFile.h"

namespace lce::file {
    
    uint8_t* InnerFile::create() const {
		return data;
	}
}
