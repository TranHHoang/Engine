#pragma once
#include <memory>
#include <type_traits> // std::forward
#include <utility>     // std::move

namespace Engine {
template <typename T>
using Unique = std::unique_ptr<T>;

template <typename T, typename... Args>
Unique<T> createUnique(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename Tb, typename Ta>
Unique<Tb> staticUniqueCast(Unique<Ta>&& from) {
  assert(dynamic_cast<Tb*>(from.get()));
  return Unique<Tb>{static_cast<Tb*>(from.release())};
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Ref<T> createRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace Engine