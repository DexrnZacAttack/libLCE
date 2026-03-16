//
// Created by DexrnZacAttack on 5/28/2025 (ZPC-I1)
//

#ifndef BINKAFILE_H
#define BINKAFILE_H

#include "LCE/filesystem/File.h"

namespace lce::msscmp {

    class LIBLCE_API BinkaFile final : public fs::File {
      public:
        BinkaFile() = delete;

        BinkaFile(const FSObject::name_t &_name, const std::vector<uint8_t> &_data,
                  const uint32_t sampleRate, fs::Directory *parent)
            : fs::File(_name, _data, parent), m_sampleRate(sampleRate) {}

        [[nodiscard]] uint32_t getSampleRate() const { return m_sampleRate; }
        void setSampleRate(const uint32_t _sampleRate) {
            m_sampleRate = _sampleRate;
        }

      private:
        uint32_t m_sampleRate;
    };

} // namespace lce::msscmp

#endif // BINKAFILE_H
