//
// Created by DexrnZacAttack on 5/28/2025 (ZPC-I1)
//

#ifndef BINKAFILE_H
#define BINKAFILE_H

#include <Filesystem/File.h>

namespace lce::msscmp {

    class LIBLCE_API BinkaFile final : public fs::File {
      public:
        BinkaFile() = delete;

        BinkaFile(const std::wstring &_name, const std::vector<uint8_t> &_data,
                  const uint32_t sampleRate, fs::Directory *parent)
            : fs::File(_name, _data, parent), mSampleRate(sampleRate) {}

        [[nodiscard]] uint32_t getSampleRate() const { return mSampleRate; }
        void setSampleRate(const uint32_t _sampleRate) {
            mSampleRate = _sampleRate;
        }

      private:
        uint32_t mSampleRate;
    };

} // namespace lce::msscmp

#endif // BINKAFILE_H
