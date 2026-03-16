//
// Created by DexrnZacAttack on 5/26/2025 (ZPC-I1)
//

#include "LCE/filesystem/FSObject.h"

#include <sstream>
#include <vector>

#include "LCE/filesystem/Directory.h"

#include "LCE/filesystem/Filesystem.h"

namespace lce::fs {
    void FSObject::remove() {
        if (!m_parent)
            return;

        // I should probably find a better way of doing this
        this->m_parent->removeChild(m_name);
        this->m_parent = nullptr;
    }

    FSObject::path_t FSObject::getPath() const {
        std::vector<const FSObject *> objs;
        const FSObject *c = this;

        while (c) {
            objs.push_back(c);

            if (!c->m_parent)
                break;

            c = c->m_parent;
        }

        FSObject::ostringstream_t oss;

        for (auto i = objs.rbegin(); i != objs.rend(); ++i) {
            if (const FSObject *obj = *i; obj->m_parent == nullptr) {
                oss << obj->getName(); // is parent is nullptr then we hit the
                                       // root directory
            } else {
                if (FSObject::name_t oName = obj->getName();
                    oName == Filesystem::ROOT ||
                    obj->m_parent->getName() == Filesystem::ROOT) {
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
