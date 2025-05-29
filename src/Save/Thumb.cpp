//
// Created by DexrnZacAttack on 3/15/2025.
//

#include <Save/Thumb.h>
#include <lodepng/lodepng.h>

namespace lce::save {
    Thumb::Thumb(std::vector<uint8_t> data, ByteOrder endian, int headerSize, bool use4ByteWideChar) {
        this->name = L"New World"; // default name

        if (headerSize != 0) {
            io::BinaryIO io((data.data()));
            if (!use4ByteWideChar) { this->name = io::BinaryIO::u16stringToWstring(io.readWChar2ByteNT(endian)); }
            else { this->name = io::BinaryIO::u32stringToWstring(io.readWChar4ByteNT(endian)); }
        }

        std::vector<uint8_t>& image_data = data;

        if (headerSize != 0)
            image_data = {data.begin() + headerSize, data.end()};

        std::vector<uint8_t> pixels;
        unsigned width, height = 64;
        lodepng::State state;

        unsigned err = lodepng::decode(pixels, width, height, state, image_data);
        if (err) std::cout << "decode error: " << err << ": " << lodepng_error_text(err) << std::endl;

        this->image = pixels;

        for (int t = 0; t < state.info_png.text_num; t++) {
            this->properties.push_back({state.info_png.text_keys[t], state.info_png.text_strings[t]});
        }
    }

    std::wstring Thumb::getWorldName() const {
        return this->name;
    }

    void Thumb::setWorldName(const std::wstring &name) {
        this->name = name;
    }

    std::vector<std::pair<std::string, std::string>> Thumb::getProperties() const {
        return this->properties;
    }

    void Thumb::setProperties(std::vector<std::pair<std::string, std::string>> properties) {
        this->properties = properties;
    }

    std::vector<uint8_t> Thumb::getImage() const {
        return this->image;
    }

    void Thumb::setImage(std::vector<uint8_t> image) {
        this->image = image;
    }
} // lce::save