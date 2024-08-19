#ifndef ARGPARSE_ARGUMENT_PARSER__H
#define ARGPARSE_ARGUMENT_PARSER__H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "bits/ArrayView.h"
#include "bits/Debug.h"
#include "bits/OrderedDict.h"
#include "bits/ParseValue.h"
#include "bits/Strings.h"
#include "bits/Types.h"

namespace argparse {
class ArgumentParser {
 public:
  struct OptionDesc {
    std::optional<std::string_view> long_name;
    std::optional<std::string_view> short_name;
    std::optional<std::string_view> help;
  };
  struct ArgumentDesc {
    std::string_view name;
    bool required{false};
  };

 private:
  struct OptionInfo {
    const OptionDesc desc;

    virtual Inputs parse(Inputs arr) = 0;
    virtual bool match(const std::type_info &) const = 0;

    OptionInfo(const OptionDesc &desc) : desc{desc} {}
    virtual ~OptionInfo() = default;
  };

  template <typename T>
  struct OptionInfoImpl final : public OptionInfo {
    using OptionInfo::OptionInfo;

    T data;

    Inputs parse(Inputs inputs) override {
      if constexpr (std::is_same_v<T, bool>) {
        data = true;
        return inputs.drop(1);
      }
      // The form '-X=1' is not supported yet, so always skip.
      inputs = inputs.drop(1);
      // Use ADL to call the correct version.
      parse_value(inputs, data);
      return inputs;
    }

    bool match(const std::type_info &ti) const override {
      return typeid(T) == ti;
    }
  };

  struct ArgumentInfo {
    const ArgumentDesc desc;

    virtual Inputs parse(Inputs args) = 0;
    virtual bool has_type(const std::type_info &) const = 0;

    ArgumentInfo(const ArgumentDesc &desc) : desc{desc} {}
    virtual ~ArgumentInfo() = default;
  };

  template <typename T>
  struct ArgumentInfoImpl final : public ArgumentInfo {
    using ArgumentInfo::ArgumentInfo;

    T data;

    Inputs parse(Inputs inputs) override {
      // Use ADL to call the correct version.
      parse_value(inputs, data);
      return inputs;
    }
    bool has_type(const std::type_info &ti) const override {
      return typeid(T) == ti;
    }
  };

  struct Errors {
    bool any{false};
    std::vector<std::string_view> unrecognized;

    void add_unrecognized(std::string_view x) {
      std::cout << __FUNCTION__ << ' ' << x << '\n';
      unrecognized.push_back(x);
      any = true;
    }
  };

 public:
  template <typename T>
  void add_option(const OptionDesc &desc) {
    if (desc.long_name && is_duplicate_name(desc.long_name.value())) return;
    if (desc.short_name && is_duplicate_name(desc.short_name.value())) return;

    auto *arg = new_option<T>(desc);
    if (desc.long_name) _opts.emplace(desc.long_name.value(), arg);
    if (desc.short_name) _opts.emplace(desc.short_name.value(), arg);
  }

  void add_flag(const OptionDesc &desc) { add_option<bool>(desc); }

  template <typename T>
  void add_argument(const ArgumentDesc &desc) {
    if (is_duplicate_name(desc.name)) return;

    auto *arg = new_argument<T>(desc);
    _args.emplace(desc.name, arg);
  }

  template <typename T>
  const T &get(std::string_view name) {
    assert(!_errs.any);

    if (auto it = _opts.find(name); it != _opts.end()) {
      auto *_info = it->second;
      assert(_info->match(typeid(T)));
      auto *info = static_cast<OptionInfoImpl<T> *>(_info);
      return info->data;
    }
    if (auto it = _args.find(name); it != _args.end()) {
      auto *_info = it->value;
      assert(_info->has_type(typeid(T)));
      auto *info = static_cast<ArgumentInfoImpl<T> *>(_info);
      return info->data;
    }
    assert(false);
  }

  void parse(int argc, char *argv[]) {
    std::vector<ArgumentInfo *> all_args;
    std::unordered_set<std::string_view> all_required;
    for (const auto [name, arg] : _args) {
      all_args.emplace_back(arg);
      if (arg->desc.required) all_required.emplace(name);
    }
    ARGPARSE_DEBUG(all_required);
    std::size_t arg_index = 0;

    std::vector<std::string_view> gc;
    gc.reserve(argc - 1);
    for (int i = 1; i < argc; i++) {
      gc.emplace_back(argv[i]);
    }
    auto inputs = Inputs{gc.data(), gc.size()};
    while (!inputs.empty()) {
      auto first = inputs[0];

      if (util::strings::starts_with(first, "-")) {
        std::string_view first_name;
        if (first.length() == 2 && first[1] == '-')
          assert(false && "'--' is not supported yet");
        if (first.length() > 2 && first[1] == '-')
          first_name = first.substr(2);
        else
          first_name = first.substr(1);
        auto it = _opts.find(first_name);
        if (it == _opts.end()) {
          _errs.add_unrecognized(first);
          inputs = inputs.drop(1);
          continue;
        }
        inputs = it->second->parse(inputs);
      } else {
        if (arg_index >= all_args.size()) {
          // Too many arguments.
          return;
        }
        auto *info = all_args[arg_index];
        inputs = info->parse(inputs);
        ARGPARSE_DEBUG(info->desc.name);
        all_required.erase(info->desc.name);
        arg_index++;
      }
    }

    // Report errors.
    assert(all_required.empty());
    if (_errs.any) {
      for (auto name : _errs.unrecognized) {
        std::cerr << "\x1b[1;31merror:\x1b[0m unrecognized option '" << name
                  << "'\n";
      }
    }
  }

 private:
  template <typename T>
  OptionInfo *new_option(const OptionDesc &desc) {
    auto opt_owned = std::make_unique<OptionInfoImpl<T>>(desc);
    auto *opt = opt_owned.get();
    _opts_gc.emplace_back(std::move(opt_owned));
    return opt;
  }

  template <typename T>
  ArgumentInfo *new_argument(const ArgumentDesc &desc) {
    auto arg_owned = std::make_unique<ArgumentInfoImpl<T>>(desc);
    auto *arg = arg_owned.get();
    _args_gc.emplace_back(std::move(arg_owned));
    return arg;
  }

  bool is_duplicate_name(std::string_view name) {
    return _opts.count(name) || _args.contains(name);
  }

 private:
  std::unordered_map<std::string_view, OptionInfo *> _opts;
  std::vector<std::unique_ptr<OptionInfo>> _opts_gc;
  bits::util::OrderedDict<std::string_view, ArgumentInfo *> _args;
  std::vector<std::unique_ptr<ArgumentInfo>> _args_gc;

  Errors _errs;
};

}  // namespace argparse

#endif  // ARGPARSE_ARGUMENT_PARSER__H
