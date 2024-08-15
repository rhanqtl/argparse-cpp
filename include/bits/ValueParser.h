#ifndef ARGPARSE_BITS_VALUE_PARSER__H
#define ARGPARSE_BITS_VALUE_PARSER__H

#include <cstdlib>
#include <string>
#include <string_view>

#include "bits/ArrayView.h"

namespace argparse {
class ValueParser {
 private:
  using Args = util::ArrayView<std::string_view>;

 public:
  template <typename T>
  void parse(Args args, T &value);

  void parse(Args args, int &value) {
    if (args.empty()) {
      value = 0;
      return;
    }
    // TODO(rhanqtl): check the form '123abc'
    value = std::atoi(args[0].data());
  }
  void parse(Args args, std::string &value) {
    if (args.empty()) {
      value = "";
      return;
    }
    value = args[0];
  }
};
}  // namespace argparse

#endif  // ARGPARSE_BITS_VALUE_PARSER__H
