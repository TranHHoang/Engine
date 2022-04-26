#pragma once
#include <unordered_map>

namespace Engine {
template <typename K, typename V>
class Map {
public:
  const V& operator[](const K& value) const { return _map[value]; }
  V& operator[](const K& value) { return _map[value]; }
  bool contains(const K& value) { return _map.contains(value); }

private:
  std::unordered_map<K, V> _map;
};
} // namespace Engine