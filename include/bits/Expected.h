#ifndef ARGPARSE_BITS_EXPECTED__H
#define ARGPARSE_BITS_EXPECTED__H

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace argparse::util {
template <typename T, typename E>
class Result {
 private:
  struct error_t {};

 public:
  template <typename... Args>
  static Result OK(Args &&...args) {
    return Result{std::forward<Args>(args)...};
  }

  template <typename... Args>
  static Result Err(Args &&...args) {
    return Result{error_t{}, std::forward<Args>(args)...};
  }

 private:
  template <typename... Args>
  /* implicit */ Result(Args &&...args) : _ok(true) {
    new (_buf) T{std::forward<Args>(args)...};
  }

  template <typename... Args>
  /* implicit */ Result(error_t, Args &&...args) : _ok{false} {
    new (_buf) E{std::forward<Args>(args)...};
  }

 public:
  template <typename U, typename G,
            typename = std::enable_if_t<std::is_convertible_v<U, T> &&
                                        std::is_convertible_v<G, E>>>
  Result(const Result &other) : _ok{other.ok()} {
    if (ok())
      new (_buf) T{other.get_unchecked()};
    else
      new (_buf) E{other.error_unchecked()};
  }
  template <typename U, typename G,
            typename = std::enable_if_t<std::is_convertible_v<U, T> &&
                                        std::is_convertible_v<G, E>>>
  Result(Result &&other) : _ok{other.ok()} {
    if (ok())
      new (_buf) T{std::move(other.get_unchecked())};
    else
      new (_buf) E{std::move(other.error_unchecked())};
  }

  // Result &operator=(const Result &other) {}
  // Result &operator=() {}

 public:
  bool ok() const { return _ok; }
  operator bool() const { return ok(); }

  const T &get_unchecked() const {
    assert(ok());
    return *reinterpret_cast<T *>(_buf);
  }
  T &get_unchecked() {
    return const_cast<T &>(static_cast<const Result *>(this)->get_unchecked());
  }
  const E &error_unchecked() const {
    assert(!ok());
    return *reinterpret_cast<E *>(_buf);
  }
  E &error_unchecked() {
    return const_cast<T &>(
        static_cast<const Result *>(this)->error_unchecked());
  }

 private:
  static constexpr std::size_t SIZE = std::max(sizeof(T), sizeof(E));
  static constexpr std::size_t ALIGNMENT = std::max(alignof(T), alignof(E));

  alignas(ALIGNMENT) std::uint8_t _buf[SIZE];
  const bool _ok;
};
}  // namespace argparse::util

#endif  // ARGPARSE_BITS_EXPECTED__H
