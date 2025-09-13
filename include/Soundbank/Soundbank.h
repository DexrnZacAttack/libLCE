//
// Created by Boreal on 05/20/2025.
//

#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include <Filesystem/Filesystem.h>
#include <IO/BinaryIO.h>
#include <Soundbank/BinkaFile.h>
#include <libLCE.h>
#include <stdexcept> //Remove when implemented
#include <vector>

namespace lce::msscmp {

    class LIBLCE_API Soundbank : public fs::Filesystem {
      public:
        enum Type {
            OLD_GEN, // read uint32_t
            NEW_GEN  // read uint64_t
        };

        explicit Soundbank(uint8_t *data);
        explicit Soundbank(std::vector<uint8_t> data);

        static bool isSoundbank(uint8_t *data);

        [[nodiscard]] uint8_t *serialize() const {
            throw std::logic_error("Function not yet implemented");
        }

      private:
        io::ByteOrder mByteOrder;
        Type mType;

        static uint64_t readUintByType(io::BinaryIO &io,
                                       const io::ByteOrder byteOrder,
                                       const Type type) {
            return type == NEW_GEN ? io.read<uint64_t>(byteOrder)
                                   : io.read<uint32_t>(byteOrder);
        }

        uint32_t mIndex2Size;
    };
} // namespace lce::msscmp

#endif
