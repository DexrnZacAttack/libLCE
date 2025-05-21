//
// Created by Boreal on 05/20/2025.
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../libLCE.h"

namespace lce::fs {

	class LIBLCE_API Filesystem {
	public:
		Filesystem() = default;
		virtual ~Filesystem();
		
		virtual uint64_t getSize() const = 0;
		virtual uint8_t* create() const = 0;
	};
}

#endif //FILE_H
