//
// Created by DexrnZacAttack on 3/15/2025.
//

#include <Save/Thumb.h>
#include <lodepng/lodepng.h>

#include <utility>

namespace lce::save {
    Thumb::Thumb(std::vector<uint8_t> data, const io::ByteOrder byteOrder,
                 const int headerSize, const bool use4ByteWideChar) {
        this->mName = L"New World"; // default name

        if (headerSize != 0) {
            io::BinaryIO io((data.data()));
            if (!use4ByteWideChar) {
                this->mName = io::BinaryIO::u16stringToWstring(
                    io.readWChar2ByteNT(byteOrder));
            } else {
                this->mName = io::BinaryIO::u32stringToWstring(
                    io.readWChar4ByteNT(byteOrder));
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

        this->mImage = pixels;

        for (int t = 0; t < state.info_png.text_num; t++) {
            this->mProperties.emplace(state.info_png.text_keys[t],
                                      state.info_png.text_strings[t]);
        }
    }

    std::wstring Thumb::getWorldName() const { return this->mName; }

    void Thumb::setWorldName(const std::wstring &name) { this->mName = name; }

    std::unordered_map<std::string, std::string> Thumb::getProperties() const {
        return this->mProperties;
    }

    void Thumb::setProperties(
        std::unordered_map<std::string, std::string> properties) {
        this->mProperties = std::move(properties);
    }

    std::vector<uint8_t> Thumb::getImage() const { return this->mImage; }

    void Thumb::setImage(std::vector<uint8_t> image) {
        this->mImage = std::move(image);
    }
} // namespace lce::save
