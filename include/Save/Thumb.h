//
// Created by DexrnZacAttack on 3/15/2025.
//

#ifndef THUMB_H
#define THUMB_H
#include <string>
#include <unordered_map>
#include <vector>
#include <IO/ByteEnums.h>
#include <IO/BinaryIO.h>
#include <libLCE.h>

namespace lce::save {

class LIBLCE_API Thumb {
protected:
    std::wstring name;
    std::vector<std::pair<std::string, std::string>> properties;
    std::vector<uint8_t> image;
public:

    explicit Thumb(std::vector<uint8_t> data, ByteOrder endian = LITTLE, int headerSize = 0x100, bool use4ByteWideChar = false);

    std::vector<uint8_t> create();

    [[nodiscard]] std::wstring getWorldName() const;
    void setWorldName(const std::wstring& name);
    [[nodiscard]] std::vector<std::pair<std::string, std::string>> getProperties() const;
    void setProperties(std::vector<std::pair<std::string, std::string>> properties);
    [[nodiscard]] std::vector<uint8_t> getImage() const;
    void setImage(std::vector<uint8_t> image);
};

} // lce::save

#endif //THUMB_H
