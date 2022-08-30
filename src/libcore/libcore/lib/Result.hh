#pragma once
#include <functional>
#include <type_traits>
#include <variant>

namespace Engine {
template <typename T, typename E>
class Result {
public:
  Result() = default;
  Result(T ok) : _data{ok} {}
  Result(E error) : _data{error} {}

  // bool matchOk(T* result) { return *result = std::get_if<T>(&_data); }
  const T& value() const { return std::get<T>(_data); }
  const E& error() const { return std::get<E>(_data); }

  template <typename Fn, typename U = std::invoke_result_t<Fn, T>>
  Result<U, E> map(Fn fn) {
    if (auto result = std::get_if<T>(&_data)) {
      return Result<U, E>{std::invoke(fn, result)};
    }
    return *this;
  }

  explicit operator bool() const { return std::holds_alternative<T>(_data); }

private:
  std::variant<T, E> _data;
};
} // namespace Engine