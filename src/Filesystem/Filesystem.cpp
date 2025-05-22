//
// Created by Boreal on 5/20/2025.
//

#include "Filesystem.h"

namespace lce::fs {
	
	Filesystem::~Filesystem() {}
	
	void Filesystem::addFile(const std::shared_ptr<File> file) {
        this->index.push_back(file);
        
        if(index.size() > indexCount) {
			setIndexCount(getIndexCount() + 1); // JAAJ???
		}
    }

    void Filesystem::removeFile(const uint32_t index) {
        const auto file = this->index[index];

        this->index.erase(this->index.begin() + index);
    }
    
    void Filesystem::setIndexCount(uint32_t count) {
		index.reserve(count);
		indexCount = count;
    }
    
	std::shared_ptr<File> Filesystem::getFileByName(const std::string name) const {
		auto find = std::find_if(index.begin(), index.end(), [&name](const std::shared_ptr<File>& file) {
			return file->getName() == name;
		});

		if (find != index.end()) {
			return *find;  // deref the iterator to get the shared_ptr
		}

		return nullptr;
	}

}
