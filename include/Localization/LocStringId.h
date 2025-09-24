//
// Created by DexrnZacAttack on 9/24/25 using zPc-i2.
//
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

/** Just some typedef so I don't have to copy that around */
typedef std::reference_wrapper<const uint32_t> LocStringId;

/** Hashes the LocStringId */
struct LocIdHash {
    size_t operator()(const LocStringId &i) const {
        return std::hash<int>{}(i.get());
    }
};

/** Returns true if both values are equal (like `operator==`) */
struct LocIdKeyEqual {
    bool operator()(const LocStringId &lhs, const LocStringId &rhs) const {
        return lhs.get() == rhs.get();
    }
};
