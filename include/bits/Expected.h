#ifndef ARGPARSE_BITS_EXPECTED__H
#define ARGPARSE_BITS_EXPECTED__H

#include <variant>

namespace argparse::util {
template <typename T, typename E>
class Expected {
 private:
  std::variant<T, E> _data;
};
}  // namespace argparse::util

#endif  // ARGPARSE_BITS_EXPECTED__H
