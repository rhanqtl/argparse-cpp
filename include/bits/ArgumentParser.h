#ifndef ARGPARSE_ARGUMENT_PARSER__H
#define ARGPARSE_ARGUMENT_PARSER__H

#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "bits/ArrayView.h"
#include "bits/Strings.h"
#include "bits/ValueParser.h"

namespace argparse {
class ArgumentParser {
 private:
  using Args = util::ArrayView<std::string_view>;

  struct ArgumentInfo {
    virtual Args parse(Args arr) = 0;
    virtual bool match(const std::type_info &) const = 0;
    virtual ~ArgumentInfo() = default;
  };

  template <typename T>
  struct ArgumentInfoImpl : public ArgumentInfo {
    T data;

    Args parse(Args tokens) override {
      // The form '-X=1' is not supported yet, so always skip.
      tokens = tokens.drop(1);
      ValueParser vp;
      vp.parse(tokens, data);
      return tokens.drop(1);
    }

    bool match(const std::type_info &ti) const override {
      return typeid(T) == ti;
    }
  };

 public:
  struct ArgumentDesc {
    std::optional<std::string_view> long_name;
    std::optional<std::string_view> short_name;
    std::optional<std::string_view> help;
  };

  struct Errors {
    bool any{false};
    std::vector<std::string_view> unrecognized;

    void add_unrecognized(std::string_view x) {
      unrecognized.push_back(x);
      any = true;
    }
  };

 public:
  template <typename T>
  void add_argument(ArgumentDesc &&desc) {
    auto *arg = new_argument<T>();
    if (desc.long_name) _args.emplace(desc.long_name.value(), arg);
    if (desc.short_name) _args.emplace(desc.short_name.value(), arg);
  }

  template <typename T>
  const T &get(std::string_view name) {
    assert(!_errs.any);

    auto *_info = _args.at(name);
    assert(_info->match(typeid(T)));
    auto *info = static_cast<ArgumentInfoImpl<T> *>(_info);
    return info->data;
  }

  void parse(int argc, char *argv[]) {
    std::cout << argc << '\n';
    std::vector<std::string_view> gc;
    gc.reserve(argc - 1);
    std::cout << gc.capacity() << '\n';
    for (int i = 1; i < argc; i++) {
      std::cout << argv[i] << '\n';
      gc.emplace_back(argv[i]);
    }
    Args args{gc.data(), gc.size()};
    std::cout << args << '\n';
    while (!args.empty()) {
      std::cout << args << '\n';
      auto first = args[0];
      std::string_view first_name;
      if (util::strings::starts_with(first, "--")) {
        first_name = first.substr(2);
      } else if (util::strings::starts_with(first, "-")) {
        first_name = first.substr(1);
      }
      auto it = _args.find(first_name);
      if (it == _args.end()) {
        _errs.add_unrecognized(first);
        continue;
      }
      args = it->second->parse(args);
    }

    if (_errs.any) {
      for (auto name : _errs.unrecognized) {
        std::cerr << "\x1b[1;31merror:\x1b[0m unrecognized argument '" << name
                  << "'\n";
      }
    }
  }

 private:
  template <typename T>
  ArgumentInfo *new_argument() {
    auto arg_owned = std::make_unique<ArgumentInfoImpl<T>>();
    auto *arg = arg_owned.get();
    _gc.emplace_back(std::move(arg_owned));
    return arg;
  }

 private:
  std::unordered_map<std::string_view, ArgumentInfo *> _args;
  std::vector<std::unique_ptr<ArgumentInfo>> _gc;

  Errors _errs;
};

}  // namespace argparse

#endif  // ARGPARSE_ARGUMENT_PARSER__H
