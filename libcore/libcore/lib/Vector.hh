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
  Vector(std::initializer_list<T>&& list) : _vec(std::move(list)) {}
  Vector(const std::initializer_list<T>& list) : _vec(list) {}

  void reserve(size_t size) { _vec.reserve(size); }
  void resize(size_t size) { _vec.resize(size); }
  void add(const T& element) { _vec.push_back(element); }
  void add(T&& element) { _vec.push_back(std::move(element)); }
  size_t size() const { return _vec.size(); }
  bool empty() const { return _vec.empty(); }
  void clear() { _vec.clear(); }

  auto begin() { return _vec.begin(); }
  auto begin() const { return _vec.begin(); }

  auto end() { return _vec.end(); }
  auto end() const { return _vec.end(); }

  T& operator[](size_t index) { return _vec[index]; }
  const T& operator[](size_t index) const { return _vec[index]; }

  const T* data() const { return _vec.data(); }

private:
  std::vector<T> _vec;
};
} // namespace Engine