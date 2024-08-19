#ifndef ARGPARSE_BITS_TYPES__H
#define ARGPARSE_BITS_TYPES__H

#include <optional>
#include <string>
#include <string_view>

#include "bits/ArrayView.h"
#include "bits/Expected.h"

namespace argparse {

using Inputs = util::ArrayView<std::string_view>;
using ParsingError = std::optional<std::string>;
// using ParsingResult = util::Result<void, ParsingError>;

}  // namespace argparse

#endif  // ARGPARSE_BITS_TYPES__H
