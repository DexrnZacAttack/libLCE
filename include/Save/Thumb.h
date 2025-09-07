//
// Created by DexrnZacAttack on 3/15/2025.
//

#ifndef THUMB_H
#define THUMB_H
#include <IO/BinaryIO.h>
#include <libLCE.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace lce::save {

    class LIBLCE_API Thumb {
      public:
        explicit Thumb(std::vector<uint8_t> data,
                       io::ByteOrder byteOrder = io::ByteOrder::LITTLE,
                       int headerSize = 0x100, bool use4ByteWideChar = false);

        [[nodiscard]] std::wstring getWorldName() const;
        void setWorldName(const std::wstring &name);
        [[nodiscard]] std::unordered_map<std::string, std::string>
        getProperties() const;
        void
        setProperties(std::unordered_map<std::string, std::string> properties);
        [[nodiscard]] std::vector<uint8_t> getImage() const;
        void setImage(std::vector<uint8_t> image);

      protected:
        std::wstring mName;
        std::unordered_map<std::string, std::string> mProperties;
        std::vector<uint8_t> mImage;
    };

} // namespace lce::save

#endif // THUMB_H
