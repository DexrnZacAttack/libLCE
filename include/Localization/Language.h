//
// Created by Boreal on 05/15/2025.
//

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "IO/Serializable.h"

#include <IO/BinaryIO.h>
#include <libLCE.h>

#include <string>
#include <utility>
#include <vector>

namespace lce::loc {

    // TODO: redo this with unordered_map with key/v for each string (it's
    // actually hashed afaik but nobody knows the algo)
    class LIBLCE_API Language : public io::Serializable {
      public:
        explicit Language(io::BinaryIO &io);
        Language(const uint8_t _byte, const uint32_t _shouldReadByte,
                 std::string _code, const uint32_t _stringCount,
                 std::vector<std::string> _strings)
            : byte(_byte), shouldReadByte(_shouldReadByte),
              code(std::move(_code)), stringCount(_stringCount),
              strings(std::move(_strings)) {}

        size_t getSize() const override;
        uint8_t *serialize() const override;

        uint8_t byte;
        uint32_t shouldReadByte;

        std::string code;
        uint32_t stringCount;
        std::vector<std::string> strings;
    };
} // namespace lce::loc

#endif // LANGUAGE_H
