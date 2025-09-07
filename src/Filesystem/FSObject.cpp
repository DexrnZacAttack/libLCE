//
// Created by DexrnZacAttack on 5/26/2025 (ZPC-I1)
//

#include <Filesystem/FSObject.h>

#include <sstream>
#include <vector>

#include <Filesystem/Directory.h>

#include "Filesystem/Filesystem.h"

namespace lce::fs {
    std::wstring FSObject::getPath() const {
        std::vector<const FSObject *> objs;
        const FSObject *c = this;

        while (c) {
            objs.push_back(c);
            c = c->parent;
        }

        std::wostringstream oss;

        for (auto i = objs.rbegin(); i != objs.rend(); ++i) {
            if (const FSObject *obj = *i; obj->parent == nullptr) {
                oss << obj->getName(); // is parent is nullptr then we hit the
                                       // root directory
            } else {
                if (std::wstring oName = obj->getName();
                    oName == Filesystem::ROOT ||
                    obj->parent->getName() == Filesystem::ROOT) {
                    oss << oName; // if parent is root or the current object is
                                  // root then we don't want double path
                                  // delimiters
                } else {
                    oss << Filesystem::ROOT << oName;
                }
            }
        }

        return oss.str();
    }
} // namespace lce::fs
