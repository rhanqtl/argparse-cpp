#ifndef ARGPARSE_BITS_VALUE_PARSER__H
#define ARGPARSE_BITS_VALUE_PARSER__H

#include <cstdlib>
#include <string>

#include "bits/Types.h"

namespace argparse {

template <typename T>
ParsingError parse_value(Inputs &inputs, T &value);

inline ParsingError parse_value(Inputs &inputs, int &value) {
  if (inputs.empty()) {
    value = 0;
  } else {
    // TODO(rhanqtl): check the form '123abc'
    value = std::atoi(inputs[0].data());
    inputs = inputs.drop(1);
  }
  return {};
}

inline ParsingError parse_value(Inputs &inputs, std::string &value) {
  if (inputs.empty()) {
    value = "";
  } else {
    value = inputs[0];
    inputs = inputs.drop(1);
  }
  return {};
}

}  // namespace argparse

#endif  // ARGPARSE_BITS_VALUE_PARSER__H
