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
	
}
