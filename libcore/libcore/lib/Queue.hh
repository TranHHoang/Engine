#pragma once
#include <queue>
#include <utility>

namespace Engine {
template <typename T>
class Queue {
public:
  void push(T&& value) { _queue.push(std::move(value)); }
  void push(const T& value) { _queue.push(value); }

  bool empty() const { return _queue.empty(); }
  const T& peek() const { return _queue.back(); }

  T pop() {
    T value = std::move(_queue.back());
    _queue.pop();
    return value;
  }

private:
  std::queue<T> _queue;
};
} // namespace Engine