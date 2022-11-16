#pragma once
#include <unordered_map>

namespace Engine {
template <typename K, typename V>
class HashMap {
public:
  const V& operator[](const K& value) const { return _map[value]; }
  V& operator[](const K& value) { return _map[value]; }
  bool contains(const K& value) { return _map.contains(value); }

  auto begin() { return _map.begin(); }
  auto begin() const { return _map.begin(); }
  auto end() { return _map.end(); }
  auto end() const { return _map.end(); }

private:
  std::unordered_map<K, V> _map;
};
} // namespace Engine