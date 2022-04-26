#pragma once

namespace Engine {
template <typename T, typename E>
class Result {
private:
  union {
    T ok;
    E error;
  };
  bool isOk;

  template <typename T, typename E>
  friend Result<T, E> ok(T x);
  template <typename T, typename E>
  friend Result<T, E> error(E x);

  // template <FnL, FnR>
  // matchLeft() {}
};

template <typename T, typename E>
Result<T, E> ok(T x) {
  Result<T, E> e;
  e.ok = x;
  return e;
}

template <typename T, typename E>
Result<T, E> error(E x) {
  Result<T, E> e;
  e.error = x;
  return e;
}
} // namespace Engine