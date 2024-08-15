#ifndef ARGPARSE_BITS_UTIL_STRINGS__H
#define ARGPARSE_BITS_UTIL_STRINGS__H

#include <string_view>

namespace argparse::util::strings {
inline bool starts_with(std::string_view s, std::string_view prefix) {
#if __cplusplus >= 202002L
  return s.starts_with(prefix);
#else
  return (s.length() >= prefix.length()) &&
         (s.substr(0, prefix.length()) == prefix);
#endif
}
}  // namespace argparse::util::strings

#endif  // ARGPARSE_BITS_UTIL_STRINGS__H
