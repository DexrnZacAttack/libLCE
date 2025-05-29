//
// Created by Boreal on 05/20/2025.
//

#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include <libLCE.h>
#include <IO/ByteEnums.h>
#include <Filesystem/Filesystem.h>
#include <Soundbank/BinkaFile.h>
#include <IO/BinaryIO.h>
#include <vector>
#include <stdexcept> //Remove when implemented

namespace lce::msscmp {
	
	class LIBLCE_API Soundbank : public fs::Filesystem {
	public:
		enum Generation {
			OLD_GEN, // read uint32_t
			NEW_GEN // read uint64_t
		};

		Soundbank(uint8_t* data);

		uint8_t* toData() const { std::logic_error("Function not yet implemented"); return nullptr; }
	private:
		ByteOrder byteOrder;
		Generation gen;

		static uint64_t readUintByGeneration(io::BinaryIO &io, ByteOrder endian, Generation gen) {
			return gen == NEW_GEN ? io.read<uint64_t>(endian) : io.read<uint32_t>(endian);
		}

		uint32_t index2Size;
	};
}

#endif
