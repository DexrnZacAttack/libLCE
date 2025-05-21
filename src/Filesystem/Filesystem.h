//
// Created by Boreal on 05/20/2025.
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../libLCE.h"
#include "File.h"
#include <vector>

namespace lce::fs {
	
	class LIBLCE_API Filesystem {
	public:
		Filesystem() = default;
		virtual ~Filesystem();
		
		Filesystem(std::vector<File>& index) : index(index) {}
		
		virtual uint64_t getSize() const = 0;
		virtual uint8_t* create() const = 0;
		
		void addFile(const File& file);
		void removeFile(uint32_t index);
		
		size_t getIndexSize() const { return index.size(); }
		const std::vector<File>& getIndex() const { return index; } 
	private:
		std::vector<File> index;
	};
}

#endif //FILE_H
