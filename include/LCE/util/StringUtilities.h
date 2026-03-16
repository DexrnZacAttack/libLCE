//
// Created by DexrnZacAttack on 12/30/25 using zPc-i2.
//
#ifndef LCE_STRINGUTILITIES_H
#define LCE_STRINGUTILITIES_H
#include <string>

namespace lce::util {
    class StringUtilities {
      public:
        template <typename CharT>
        constexpr static std::basic_string<CharT> trimEndNullBytes(const std::basic_string_view<CharT> &in) {
            size_t len = in.size();
            while (len && in[len - 1] == 0)
                --len;

            return std::basic_string(in.begin(), in.begin() + len);
        }

        template <typename CharT>
        constexpr static std::basic_string_view<CharT> nullBytesTrimmedView(const std::basic_string_view<CharT> &in) {
            size_t len = in.size();
            while (len && in[len - 1] == 0)
                --len;

            return std::basic_string_view(in.begin(), in.begin() + len);
        }
    };
} // namespace lce::util

#endif // LCE_STRINGUTILITIES_H