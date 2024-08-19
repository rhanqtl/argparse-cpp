#ifndef ARGPARSE_BITS_UTIL_ORDERED_DICT__H
#define ARGPARSE_BITS_UTIL_ORDERED_DICT__H

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

namespace argparse::bits::util {
template <typename K, typename V, typename Hash = std::hash<K>,
          typename Eq = std::equal_to<K>>
class OrderedDict {
  struct Entry {
    const K key;
    V value;

    Entry(const K &key, const V &value) : key{key}, value{value} {}
    Entry(K &&key, V &&value) : key{std::move(key)}, value{std::move(value)} {}
  };

 public:
  struct iterator {
    typename std::vector<Entry>::iterator it;

    /* implicit */ iterator(typename std::vector<Entry>::iterator pos)
        : it{pos} {}

    Entry &operator*() { return *it; }
    Entry *operator->() { return it.operator->(); }

    iterator &operator++() {
      ++it;
      return *this;
    }
    iterator operator++(int) {
      auto ans = *this;
      operator++();
      return ans;
    }

    bool operator==(const iterator &other) const { return it == other.it; }
    bool operator!=(const iterator &other) const { return !operator==(other); }
  };

 public:
  OrderedDict() = default;
  OrderedDict(const OrderedDict &) = default;
  OrderedDict(OrderedDict &&) = default;

  OrderedDict &operator=(const OrderedDict &) = default;
  OrderedDict &operator=(OrderedDict &&) = default;

  ~OrderedDict() = default;

 public:
  iterator begin() { return _entries.begin(); }
  iterator end() { return _entries.end(); }

 public:
  bool contains(const K &key) const {
    return std::any_of(_entries.begin(), _entries.end(),
                       [&key](const Entry &ent) { return ent.key == key; });
  }
  iterator find(const K &key) {
    return std::find_if(_entries.begin(), _entries.end(),
                        [&key](const Entry &ent) { return ent.key == key; });
  }

  V &operator[](const K &key);
  const V &operator[](const K &key) const;

  template <typename... Args>
  void emplace(Args &&...args) {
    _entries.emplace_back(std::forward<Args>(args)...);
  }

 private:
  std::vector<Entry> _entries;
};
}  // namespace argparse::bits::util

#endif  // ARGPARSE_BITS_UTIL_ORDERED_DICT__H
