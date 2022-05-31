#pragma once

namespace Engine {
template <typename... Ts> // (7)
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

template <typename T>
inline bool null(const T* v) {
  return v == nullptr;
}
} // namespace Engine