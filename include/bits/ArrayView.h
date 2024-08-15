#ifndef ARGPARSE_ARRAY_VIEW__H
#define ARGPARSE_ARRAY_VIEW__H

#include <cassert>
#include <cstddef>
#include <iostream>
#include <ostream>

namespace argparse::util {
template <typename T>
class ArrayView {
 public:
  ArrayView(T *data, std::size_t len) : _data{data}, _len{len} {}
  ArrayView(const ArrayView &other) = default;
  ArrayView(ArrayView &&other) = default;

  ArrayView &operator=(const ArrayView &other) = default;
  ArrayView &operator=(ArrayView &&other) = default;

  ~ArrayView() = default;

  std::size_t size() const { return _len; }
  bool empty() const { return size() == 0; }

  T &operator[](std::size_t i) { return _data[i]; }
  const T &operator[](std::size_t i) const { return _data[i]; }

  ArrayView slice(std::size_t start) const {
    assert(start <= size());
    return {_data + start, size() - start};
  }
  ArrayView drop(std::size_t n) const { return slice(n); }

  friend std::ostream &operator<<(std::ostream &os, const ArrayView &arr) {
    os << '[';
    for (std::size_t i = 0; i < arr.size(); i++) {
      if (i > 0) os << ", ";
      os << arr[i];
    }
    os << ']';
    return os;
  }

 private:
  T *_data;
  std::size_t _len;
};
}  // namespace argparse::util

#endif  // ARGPARSE_ARRAY_VIEW__H
