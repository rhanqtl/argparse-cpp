#ifndef ARGPARSE_BITS_UTIL_DEBUG__H
#define ARGPARSE_BITS_UTIL_DEBUG__H

#include <cstddef>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define ARGPARSE_DEBUG(x) \
  argparse::util::handle_debug(x, #x, __FILE__, __LINE__, __FUNCTION__)

namespace argparse::util {

template <typename T>
void print_impl(const T &x) {
  std::cout << x;
}

template <typename T>
void print_impl(const std::vector<T> &xs) {
  std::cout << '[';
  for (std::size_t i = 0; i < xs.size(); i++) {
    if (i > 0) std::cout << ", ";
    std::cout << '[' << i << ']' << " = ";
    print_impl(xs[i]);
  }
  std::cout << ']';
}

template <typename K, typename V>
void print_impl(const std::unordered_map<K, V> &m) {
  std::cout << '{';
  bool first = true;
  for (const auto &[k, v] : m) {
    if (first)
      first = false;
    else
      std::cout << ", ";

    print_impl(k);
    std::cout << " = ";
    print_impl(v);
  }
  std::cout << '}';
}

template <typename T>
void print_impl(const std::unordered_set<T> &xs) {
  std::cout << '{';
  bool first = true;
  for (const auto &x : xs) {
    if (first)
      first = false;
    else
      std::cout << ", ";
    print_impl(x);
  }
  std::cout << '}';
}

template <typename T>
void handle_debug(const T &x, const char *const x_str, const char *const file,
                  const int line, const char *const func) {
  std::cout << file << ':' << line << ' ' << func << '\t' << x_str << " = ";
  print_impl(x);
  std::cout << '\n';
}

}  // namespace argparse::util

#endif  // ARGPARSE_BITS_UTIL_DEBUG__H
