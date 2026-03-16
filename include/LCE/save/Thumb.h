//
// Created by DexrnZacAttack on 3/15/2025.
//

#ifndef THUMB_H
#define THUMB_H
#include "LCE/libLCE.h"
#include <BinaryIO/buffer/BinaryBuffer.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace lce::save {

    /** Stores the logoff thumbnail, world name, and various other world info
     *
     * Used for the world entries on the world select screen, as it is faster to
     * store the world info in the thumbnail rather than decompressing the save
     * file and reading level.dat.
     *
     * @see
     * https://team-lodestone.github.io/Documentation/LCE/File%20Types/THUMB for
     * more info
     */
    class LIBLCE_API Thumb {
      public:
        explicit Thumb(
            std::vector<uint8_t> data,
            bio::util::ByteOrder byteOrder = bio::util::ByteOrder::LITTLE,
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
        std::wstring m_name;
        std::unordered_map<std::string, std::string> m_properties;
        std::vector<uint8_t> m_image;
    };

} // namespace lce::save

#endif // THUMB_H
