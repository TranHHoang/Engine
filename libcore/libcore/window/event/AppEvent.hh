#pragma once
#include <cstdint>

namespace Engine::Window::Event {
struct WindowResized {
  WindowResized(uint32_t width, uint32_t height)
      : width{width},
        height{height} {}

  uint32_t width, height;
};

struct WindowClosed {};
struct WindowMoved {};
} // namespace Engine::Window::Event