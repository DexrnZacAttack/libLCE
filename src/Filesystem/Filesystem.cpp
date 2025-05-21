//
// Created by Boreal on 5/20/2025.
//

#include "Filesystem.h"

namespace lce::fs {
	
	Filesystem::~Filesystem() {}
	
	void Filesystem::addFile(const fs::File& file) {
        this->index.push_back(file);
    }

    void Filesystem::removeFile(const uint32_t index) {
        const fs::File file = this->index[index];

        this->index.erase(this->index.begin() + index);
    }
    
    void Filesystem::resizeTo(size_t count) {
		index.resize(count);
    }
    
    std::optional<File> Filesystem::getFileByName(std::string name) {
        const auto find = std::find_if(index.begin(), index.end(), [&name](const File& file) {
            return file.getName() == name;
        });

        if (find != index.end()) return *find;

        return std::nullopt;
    }

}
