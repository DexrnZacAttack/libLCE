//
// Created by Boreal on 05/20/2025.
//

#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include "LCE/filesystem/Filesystem.h"
#include "LCE/libLCE.h"
#include "LCE/soundbank/BinkaFile.h"
#include <BinaryIO/buffer/BinaryBuffer.h>
#include <vector>

namespace lce::msscmp {

    class LIBLCE_API Soundbank final : public fs::Filesystem {
      public:
        enum Type {
            OLD_GEN, // read uint32_t
            NEW_GEN  // read uint64_t
        };

        explicit Soundbank(uint8_t *data);
        explicit Soundbank(std::vector<uint8_t> data);

        static bool isSoundbank(uint8_t *data);

      private:
        bio::util::ByteOrder mByteOrder;
        Type mType;

        static uint64_t readUintByType(bio::buffer::BinaryBuffer &io,
                                       const bio::util::ByteOrder byteOrder,
                                       const Type type) {
            return type == NEW_GEN ? io.read<uint64_t>(byteOrder)
                                   : io.read<uint32_t>(byteOrder);
        }

        uint32_t mIndex2Size;
    };
} // namespace lce::msscmp

#endif
