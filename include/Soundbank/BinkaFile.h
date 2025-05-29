//
// Created by DexrnZacAttack on 5/28/2025 (ZPC-I1)
//

#ifndef BINKAFILE_H
#define BINKAFILE_H

#include <Filesystem/File.h>

namespace lce::msscmp {

    class LIBLCE_API BinkaFile : public fs::File {
    public:
        BinkaFile() = delete;

        BinkaFile(std::wstring name, std::vector<uint8_t> data, uint32_t sampleRate, fs::Directory* parent) :
            fs::File(name, data, parent), sampleRate(sampleRate) {}

        uint32_t getSampleRate() const { return sampleRate; }
        void setSampleRate(const uint32_t _sampleRate) { sampleRate = _sampleRate; }

    private:
        uint32_t sampleRate;
    };

} // namespace lce::msscmp

#endif // BINKAFILE_H
