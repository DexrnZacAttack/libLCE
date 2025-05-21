//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVEFILE_H
#define ARCHIVEFILE_H
#include <string>
#include "../IO/BinaryIO.h"
#include "../libLCE.h"
#include "File.h"

namespace lce::file {
    class LIBLCE_API InnerFile : File {
	public:
        InnerFile() = default;
        InnerFile(std::string name, uint64_t size, uint64_t offset, uint8_t* data) 
			: name(name), size(size), offset(offset), data(data) {} // C++ moment
        virtual ~InnerFile() {}

        InnerFile(uint8_t* data);
        
        std::string getName() const { return name; }
        uint64_t getOffset() const { return offset; }
        uint64_t getSize() const override { return size; }
        uint8_t* create() const override;
        
        void setName(std::string _name) { name = _name; }
		void setData(uint64_t _size, uint64_t _offset, uint8_t* _data);
	private:
        std::string name;
        uint64_t size;
        uint64_t offset;
		uint8_t* data;
    };
}


#endif
