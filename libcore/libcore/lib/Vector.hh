#pragma once
#include <initializer_list>
#include <utility>
#include <vector>

namespace Engine {
template <typename T>
class Vector {
public:
  Vector() = default;
  Vector(size_t count) : _vec{count} {}
  Vector(std::vector<T>&& vec) : _vec(std::move(vec)) {}
  Vector(std::initializer_list<T>&& list) : _vec(std::move(list)) {}

  void reserve(size_t size) { _vec.reserve(size); }
  void add(const T& element) { _vec.push_back(element); }
  void add(T&& element) { _vec.push_back(std::move(element)); }
  size_t size() const { return _vec.size(); }

  auto begin() { return _vec.begin(); }
  auto begin() const { return _vec.begin(); }

  auto end() { return _vec.end(); }
  auto end() const { return _vec.end(); }

  const T* data() const { return _vec.data(); }

private:
  std::vector<T> _vec;
};
} // namespace Engine