#pragma once
#include <optional>
#include <queue>
#include <type_traits>

namespace Engine {
template <typename T>
class Queue {
public:
  void push(T&& value) { _queue.push(std::move(value)); }
  void push(const T& value) { _queue.push(value); }
  bool empty() const { return _queue.empty(); }

  std::optional<T> peek() const {
    if (_queue.empty()) {
      return {};
    }
    return _queue.back();
  }

  std::optional<T> pop() {
    if (_queue.empty())
      return {};

    T value = _queue.back();
    _queue.pop();
    return value;
  }

private:
  std::queue<T> _queue;
};
} // namespace Engine