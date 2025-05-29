//
// Created by DexrnZacAttack on 5/26/2025 (ZPC-I1)
//

#include <Filesystem/FSObject.h>

#include <sstream>
#include <vector>

#include <Filesystem/Directory.h>

namespace lce::fs {
    std::wstring FSObject::getPath() const {
        std::vector<const FSObject*> objs;
        const FSObject* c = this;

        while (c) {
            objs.push_back(c);
            c = c->parent;
        }

        std::wostringstream oss;

        for (auto i = objs.rbegin(); i != objs.rend(); ++i) {
            if (const FSObject* obj = *i; obj->parent == nullptr) {
                oss << obj->getName(); // is parent is nullptr then we hit the root directory (there should not be an
                                       // orphaned file outside the filesystem*)
            } else {
                std::wstring name = obj->getName();

                if (name == L"/" || obj->parent->getName() == L"/")
                    oss << name; // if parent is root or the current object is root then we don't want double path
                                 // delimiters
                else
                    oss << L"/" << name;
            }
        }

        return oss.str();
    }
} // namespace lce::fs
