#pragma once
#include <iterator>
#include <type_traits>

#include <libcore/lib/Option.hh>

namespace Engine::ListUtils {
template <typename Cont, typename Fn>
void forEachIndexed(const Cont& cont, Fn fn) {
  for (auto it = std::begin(cont); it != std::end(cont); ++it)
    std::invoke(fn, *it, std::distance(std::begin(cont), it));
}

template <typename Cont, typename Fn>
void forEach(const Cont& cont, Fn fn) {
  forEachIndexed(cont, [&](auto v, int) { std::invoke(fn, v); });
}

template <typename Cont, typename Fn>
void transformIndexed(const Cont& cont, Fn fn) {
  for (auto it = std::begin(cont); it != std::end(cont); ++it) {
    *it = std::invoke(fn, *it, std::distance(std::begin(cont), it));
  }
}

template <typename Cont, typename Fn>
void transform(const Cont& cont, Fn fn) {
  transformIndexed(cont, [&](auto v, int) { std::invoke(fn, v); });
}

template <typename Cont, typename Fn, typename T = typename Cont::iterator>
Option<T> find(const Cont& cont, Fn fn) {
  auto it = std::find_if(std::begin(cont), std::end(cont), fn);
  return it == std::end(cont) ? Option<T>() : Option(std::move(it));
}

template <typename Cont, typename Fn, typename T = typename Cont::iterator>
int64_t findIndex(const Cont& cont, Fn fn) {
  auto it = std::find_if(std::begin(cont), std::end(cont), fn);
  return it != std::end(cont) ? std::distance(std::begin(cont), it) : -1;
}
} // namespace Engine::ListUtils