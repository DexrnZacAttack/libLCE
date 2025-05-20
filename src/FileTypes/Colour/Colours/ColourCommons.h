//
// Created by DexrnZacAttack on 1/2/2025.
//

#ifndef COLOURCOMMONS_H
#define COLOURCOMMONS_H
#include <string>
#include "../../../libLCE.h"

namespace lce::colour {

class LIBLCE_API ColourCommons {
public:
    std::string name;

    ColourCommons();
    explicit ColourCommons(std::string name);
    virtual uint32_t getSize();
};

} // lce::colour

#endif //COLOURCOMMONS_H
