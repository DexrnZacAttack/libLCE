//
// Created by Boreal on 05/20/2025.
//

#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include "../libLCE.h"
#include "../IO/ByteEnums.h"
#include "../File/File.h"
#include "../File/InnerFile.h"
#include <vector>
#include <stdexcept> //Remove when implemented

namespace lce::msscmp {
	
	class LIBLCE_API SoundbankInnerFile : public file::InnerFile {
	public:
		SoundbankInnerFile() = default;
		
        SoundbankInnerFile(std::string name, uint64_t size, uint64_t offset, uint8_t* data, uint32_t sampleRate) 
			: file::InnerFile(name, size, offset, data), sampleRate(sampleRate) {}
		
		uint32_t getSampleRate() { return sampleRate; }
		void setSampleRate(uint32_t _sampleRate) { sampleRate = _sampleRate; }
	private:
		uint32_t sampleRate;
	};
	
	class LIBLCE_API SoundbankFile : public file::File {
	public:
		SoundbankFile(uint8_t* data);
		
		uint64_t getSize() const override { std::logic_error("Function not yet implemented"); return 0;  }
		uint8_t* create() const override { std::logic_error("Function not yet implemented"); return nullptr; }
		
		std::vector<SoundbankInnerFile> getIndex2() const { return index2; }
	private:
		ByteOrder byteOrder;
		Generation gen;
		
		uint32_t index2Size;
		std::vector<SoundbankInnerFile> index2;
	};
}

#endif
