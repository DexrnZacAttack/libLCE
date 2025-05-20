//
// Created by Boreal on 05/20/2025.
//

#ifndef FILE_H
#define FILE_H

#include "../libLCE.h"

namespace lce::file {

	class LIBLCE_API File {
	public:
		File() = default;
		virtual ~File();
		
		virtual uint32_t getSize() const = 0;
		virtual uint8_t* create() const = 0;
		
		uint32_t size;
        uint32_t offset; // if this isn't 0, its an inner file
        uint8_t* data;
	};
}

#endif //FILE_H
