//
// Created by Boreal on 05/20/2025.
//

#include "Soundbank.h"
#include "../IO/BinaryIO.h"

#include <iostream>
#include <utility>

namespace lce::msscmp {
	
	SoundbankFile::SoundbankFile(uint8_t* data) {
		io::BinaryIO io(data);
		
		std::string magic = io.readUtf8(4);
		
		if (magic == "BANK") byteOrder = BIG;
		else if (magic == "KNAB") byteOrder = LITTLE;
		
		io.seek(0x18);
		uint32_t firstVal = io.read<uint32_t>(byteOrder);
		uint32_t secondVal = io.read<uint32_t>(byteOrder);
		gen = (firstVal != secondVal) ? NEW_GEN : OLD_GEN; // if the two uint32_t's match, its old gen. idk why.

		io.seek(0x04); // go back
		
		io.read<uint32_t>(byteOrder); // Unknown "I forgot what this is"
		
		uint64_t dataStartOffset = io.readUintByGeneration(byteOrder, gen);
		
		io.read<uint64_t>(byteOrder); // Unknown

		uint64_t firstEntryOffset = io.readUintByGeneration(byteOrder, gen);

		uint64_t lastEntryOffset = io.readUintByGeneration(byteOrder, gen);
		
		io.read<uint64_t>(byteOrder); // Unknown

		if(gen == NEW_GEN) {
			uint64_t unknownNewGen = io.read<uint64_t>(byteOrder);
		}
		
		io.readUintByGeneration(byteOrder, gen);
		
		uint64_t index1Size = io.readUintByGeneration(byteOrder, gen);
		
		io.read<uint32_t>(byteOrder); // Unknown
		
		Generation opposite = (gen == NEW_GEN) ? OLD_GEN : NEW_GEN;
		index2Size = io.readUintByGeneration(byteOrder, opposite);
		
		std::string name =  io.readUtf8(12);
		
		io.seek(lastEntryOffset + 4);
		
		index2 = std::vector<SoundbankInnerFile>(index2Size);
		
		for (uint64_t i = 0; i < index2Size; i++) {
			io.seek((lastEntryOffset + 4) + (i * 8));
			
			uint32_t entryOffset = io.read<uint32_t>(byteOrder);
			
			uint32_t fStructureOffset = 0;
            if(i != index2Size) {
                fStructureOffset = io.read<uint32_t>(byteOrder);
            }
            
            io.seek(entryOffset);
            
            uint32_t nameOffset = io.read<uint32_t>(byteOrder);
            uint32_t currentOffset = io.getPosition();
            
            io.seek(nameOffset);
            std::string fileName = io.readUtf8NullTerminated();
			io.seek(currentOffset);
			
			io.read<uint32_t>(byteOrder);
			
			uint32_t dataOffset = io.read<uint32_t>(LITTLE);
			io.read<uint32_t>(byteOrder);
			io.read<uint32_t>(byteOrder);
			uint32_t sampleRate = io.read<uint32_t>(byteOrder);
			uint32_t fileSize = io.read<uint32_t>(byteOrder);
			
			int32_t oldPos = io.getPosition();

			uint8_t* data = new uint8_t[fileSize];
			io.seek(dataOffset);
			io.readInto(data, fileSize);
			io.seek(oldPos);
			
			index2[i] = SoundbankInnerFile(fileName, fileSize, dataOffset, std::move(data), sampleRate);
		}
	}
}
