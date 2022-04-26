#pragma once
#include <string>
#include <string_view>

#include <libcore/window/input/Key.hh>

namespace Engine::Window::Event {
struct KeyDown {
  KeyDown(Key key, bool isRepeat = false) : key{key}, isRepeat{isRepeat} {}
  Key key;
  bool isRepeat;
};

struct KeyUp {
  KeyUp(Key key) : key{key} {}
  Key key;
};

struct TextInput {
  TextInput(std::string_view text) : text{text} {}
  std::string text;
};
} // namespace Engine::Window::Event