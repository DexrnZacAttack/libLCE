//
// Created by Boreal on 05/20/2025.
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../libLCE.h"
#include "File.h"
#include <vector>
#include <optional>
#include <algorithm>

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
		void resizeTo(size_t size);
		
		std::optional<File> getFileByName(std::string name);
		File& getFileByIndex(size_t i) { return index[i]; }
		
		size_t getIndexSize() const { return index.size(); }
		const std::vector<File>& getIndex() const { return index; }
		std::vector<File>& getIndex() { return index; } 
	private:
		std::vector<File> index;
	};
}

#endif //FILE_H
