//
// Created by Boreal on 05/15/2025.
//

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <IO/BinaryIO.h>
#include <libLCE.h>

#include <string>
#include <vector>

namespace lce::loc {
    class LIBLCE_API Language {
    public:
        Language(io::BinaryIO& io);
        Language(uint8_t _byte, uint32_t _shouldReadByte, std::string _code, uint32_t _stringCount,
                 std::vector<std::string> _strings) :
            byte(_byte), shouldReadByte(_shouldReadByte), code(_code), stringCount(_stringCount), strings(_strings) {}

        uint32_t getSize() const;
        uint8_t* toData() const;

        uint8_t byte;
        uint32_t shouldReadByte;

        std::string code;
        uint32_t stringCount;
        std::vector<std::string> strings;
    };
} // namespace lce::loc

#endif // LANGUAGE_H
