//
// Created by DexrnZacAttack on 12/19/2024.
//

#ifndef ARCHIVEFILE_H
#define ARCHIVEFILE_H
#include <string>
#include "../IO/BinaryIO.h"
#include "../libLCE.h"
#include "File.h"

#include <vector>

namespace lce::file {
    class LIBLCE_API InnerFile : File {
	public:
        InnerFile() = default;
        InnerFile(std::u16string name, uint64_t size, uint64_t offset, uint8_t* data) 
			: name(name), size(size), offset(offset), data(data) {}
		InnerFile(std::string name, uint64_t size, uint64_t offset, uint8_t* data) 
			: size(size), offset(offset), data(data) { setName(name); }
        virtual ~InnerFile() {}
        // JAAJ in C++?
        
        std::string getName() const;
        std::u16string getNameU16() const { return name; }
        uint64_t getOffset() const { return offset; }
        uint64_t getSize() const override { return size; }
        uint8_t* create() const override;
        uint8_t* getData() { return data; }
        std::vector<uint8_t> getDataVec() const;
        
        void setName(std::string _name);
		void setNameU16(std::u16string _name) { name = _name; }
        void setSize(uint64_t _size) { size = _size; } 
        void setOffset(uint64_t _offset) {offset = _offset; }
        void setData(uint8_t* _data) { data = _data; }
        void setDataVec(std::vector<uint8_t> vector);
	
	private:
        std::u16string name;
        uint64_t size;
        uint64_t offset;
		uint8_t* data;
    };
}


#endif
