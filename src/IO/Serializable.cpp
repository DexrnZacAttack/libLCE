//
// Created by DexrnZacAttack on 9/22/25 using zPc-i2.
//
#include <fstream>

#include <IO/Serializable.h>

namespace lce::io {
    void Serializable::writeFile(const std::filesystem::path &p) const {
        std::ofstream out(p, std::ios::binary);
        out.write(reinterpret_cast<char *>(serialize()), getSize());
    }
} // namespace lce::io