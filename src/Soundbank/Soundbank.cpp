//
// Created by Boreal on 05/20/2025.
//

#include "Soundbank.h"
#include "../IO/BinaryIO.h"

#include <iostream>

namespace lce::msscmp {
	
	SoundbankFile::SoundbankFile(uint8_t* data) {
		io::BinaryIO io(data);
		
		std::string magic = io.readUtf8(4);
		std::cout << "Magic: " << magic << std::endl;
		
		if (magic == "BANK") byteOrder = BIG;
		else if (magic == "KNAB") byteOrder = LITTLE;
		
		io.seek(0x18);
		uint32_t firstVal = io.read<uint32_t>(byteOrder);
		uint32_t secondVal = io.read<uint32_t>(byteOrder);
		gen = (firstVal != secondVal) ? NEW_GEN : OLD_GEN; // if the two uint32_t's match, its old gen. idk why.

		io.seek(0x04); // go back
		
		uint32_t unknown1 = io.read<uint32_t>(byteOrder); // Unknown "I forgot what this is"
		
		uint64_t dataStartOffset = io.readUintByGeneration(byteOrder, gen);
		
		uint64_t unknown2 = io.read<uint64_t>(byteOrder); // Unknown

		uint64_t firstIndexOffset = io.readUintByGeneration(byteOrder, gen);

		uint64_t lastItemOffset = io.readUintByGeneration(byteOrder, gen);
		
		uint64_t unknown3 = io.read<uint64_t>(byteOrder); // Unknown

		if(gen == NEW_GEN) {
			uint64_t unknownNewGen = io.read<uint64_t>(byteOrder);
		}
		
		uint64_t unknownOffset = io.readUintByGeneration(byteOrder, gen);
		
		uint64_t index1Size = io.readUintByGeneration(byteOrder, gen);
		
		uint32_t unknown4 = io.read<uint32_t>(byteOrder); // Unknown
		
		uint32_t index2Size = io.read<uint32_t>(byteOrder);
		
		std::cout << std::hex << io.getPosition() << std::dec << std::endl;
		std::string name =  io.readUtf8(12);
		std::cout << "Name: " << name << std::endl;
	}
}
