//
// Created by DexrnZacAttack on 3/15/2025.
//

#include "save/Thumb.h"

#include "BinaryIO/util/string/StringConverter.h"

#include <lodepng/lodepng.h>

#include <utility>

namespace lce::save {
    Thumb::Thumb(std::vector<uint8_t> data,
                 const bio::util::ByteOrder byteOrder, const int headerSize,
                 const bool use4ByteWideChar) {
        this->m_name = L"New World"; // default name

        if (headerSize != 0) {
            bio::buffer::BinaryBuffer io((data.data()));
            if (!use4ByteWideChar) {
                this->m_name =
                    bio::util::string::StringConverter::u16stringToWstring(
                        io.readStringWithLength<char16_t>(byteOrder, bio::util::string::StringLengthEncoding::NULL_TERMINATE));
            } else {
                this->m_name =
                    bio::util::string::StringConverter::u32stringToWstring(
                    io.readStringWithLength<char32_t>(byteOrder, bio::util::string::StringLengthEncoding::NULL_TERMINATE));
            }
        }

        std::vector<uint8_t> &image_data = data;

        if (headerSize != 0)
            image_data = {data.begin() + headerSize, data.end()};

        std::vector<uint8_t> pixels;
        unsigned width, height = 64;
        lodepng::State state;

        if (const unsigned err =
                lodepng::decode(pixels, width, height, state, image_data))
            std::cout << "decode error: " << err << ": "
                      << lodepng_error_text(err) << std::endl;

        this->m_image = pixels;

        for (int t = 0; t < state.info_png.text_num; t++) {
            this->m_properties.emplace(state.info_png.text_keys[t],
                                      state.info_png.text_strings[t]);
        }
    }

    std::wstring Thumb::getWorldName() const { return this->m_name; }

    void Thumb::setWorldName(const std::wstring &name) { this->m_name = name; }

    std::unordered_map<std::string, std::string> Thumb::getProperties() const {
        return this->m_properties;
    }

    void Thumb::setProperties(
        std::unordered_map<std::string, std::string> properties) {
        this->m_properties = std::move(properties);
    }

    std::vector<uint8_t> Thumb::getImage() const { return this->m_image; }

    void Thumb::setImage(std::vector<uint8_t> image) {
        this->m_image = std::move(image);
    }
} // namespace lce::save
