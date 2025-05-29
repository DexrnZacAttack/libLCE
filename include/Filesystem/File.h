//
// Created by DexrnZacAttack on 5/27/2025.
//

#ifndef FILE_H
#define FILE_H
#include <string>
#include <libLCE.h>

#include <vector>

#include <Filesystem/FSObject.h>

namespace lce::fs {
	
	class LIBLCE_API File : public FSObject {
	public:
		File(std::wstring name, std::vector<uint8_t> data, Directory *parent)
			: FSObject(std::move(name)), data(std::move(data)) {
			this->parent = parent;
		}

		/// Gets if the FSObject is a file or not
		///
		/// Because this is a File, it will always return true
		[[nodiscard]] bool isFile() const override { return true; }

		/// Gets the file's data
		[[nodiscard]] const std::vector<uint8_t>& getData() const { return data; }
		/// Gets the file's size (equiv. getData().size())
		[[nodiscard]] size_t getSize() const { return data.size(); }

	private:
		std::vector<uint8_t> data;
	};
}


#endif
