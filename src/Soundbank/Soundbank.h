//
// Created by Boreal on 05/20/2025.
//

#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include "../File/File.h"
#include "../libLCE.h"
#include "../IO/ByteEnums.h"
#include <stdexcept> //Remove when implemented

namespace lce::msscmp {
	
	class LIBLCE_API SoundbankFile : public file::File {
	public:
		SoundbankFile(uint8_t* data);
		
		uint32_t getSize() const override { std::logic_error("Function not yet implemented"); return 0;  }
		uint8_t* create() const override { std::logic_error("Function not yet implemented"); return nullptr; }
	private:
		ByteOrder byteOrder;
		Generation gen;
	};
}

#endif
