#pragma once
#include <optional>

namespace Engine {
template <typename T>
class Option {
public:
  Option() : mValue{std::nullopt} {}
  Option(T&& value) : mValue{value} {}

  bool ifSome(T* value) const {
    if (mValue.has_value()) {
      *value = mValue.value();
      return true;
    }

    return false;
  }

private:
  std::optional<T> mValue;
};
} // namespace Engine