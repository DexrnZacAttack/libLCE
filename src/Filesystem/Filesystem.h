//
// Created by Boreal on 05/20/2025.
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../libLCE.h"
#include "File.h"
#include <vector>
#include <algorithm>
#include <memory>

namespace lce::fs {
	
	class LIBLCE_API Filesystem {
	public:
		Filesystem() = default;
		virtual ~Filesystem();
		
		Filesystem(std::vector<std::shared_ptr<File>>& index) : index(index) {}
		
		virtual uint64_t getSize() const = 0;
		virtual uint8_t* create() const = 0;
		
		void addFile(const std::shared_ptr<File> file);
		void removeFile(uint32_t index);
		void setIndexCount(uint32_t count);
		
		std::shared_ptr<File> getFileByName(std::string name) const;
		std::shared_ptr<File> getFileByIndex(size_t i) const { return index[i]; }
		
		size_t getIndexCount() const { return indexCount; }
		const std::vector<std::shared_ptr<File>>& getIndex() const { return index; }
		std::vector<std::shared_ptr<File>>& getIndex() { return index; }
		
		void setIndex(std::vector<std::shared_ptr<File>>& index) { this->index = index; }
	private:
		std::vector<std::shared_ptr<File>> index; // shared_ptr for the polymorphism ig
		uint32_t indexCount;
	};
}

#endif //FILE_H
