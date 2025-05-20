//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLORCOMMONS_H
#define COLORCOMMONS_H
#include <string>
#include "../../../libLCE.h"

namespace lce::color {

class LIBLCE_API ColorCommons {
public:
    std::string name;

    ColorCommons();
    explicit ColorCommons(std::string name);
    virtual uint32_t getSize();
};

} // lce::color

#endif //COLORCOMMONS_H
