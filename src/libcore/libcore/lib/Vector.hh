#pragma once
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <optional>
#include <utility>
#include <vector>

namespace Engine {
template <typename T>
class Vector {
public:
  using value_type = T;

  Vector() = default;
  Vector(size_t count) { _vec.reserve(count); }
  Vector(std::initializer_list<T>&& list) : _vec{std::move(list)} {}
  Vector(const std::initializer_list<T>& list) : _vec{list} {}
  Vector(const std::vector<T>& vec) : _vec{vec} {}

  void reserve(size_t size) { _vec.reserve(size); }
  void resize(size_t size) { _vec.resize(size); }
  void add(const T& element) { _vec.push_back(element); }
  void add(T&& element) { _vec.push_back(std::move(element)); }
  void clear() { _vec.clear(); }

  size_t size() const { return _vec.size(); }
  bool empty() const { return _vec.empty(); }

  auto begin() { return _vec.begin(); }
  auto begin() const { return _vec.begin(); }
  auto end() { return _vec.end(); }
  auto end() const { return _vec.end(); }

  T& operator[](size_t index) { return _vec[index]; }
  const T& operator[](size_t index) const { return _vec[index]; }

  const T* data() const { return _vec.data(); }
  T* data() { return _vec.data(); }
  std::optional<T> back() const {
    return _vec.size() > 0 ? _vec.back() : std::nullopt;
  }

  // Functional mapping
  template <typename Fn, typename U = std::invoke_result_t<Fn, T>>
  Vector<U> map(Fn fn) const {
    std::vector<U> temp{_vec.size()};
    std::transform(_vec.begin(), _vec.end(), std::back_inserter(temp), fn);
    return temp;
  }

private:
  std::vector<T> _vec;
};
} // namespace Engine