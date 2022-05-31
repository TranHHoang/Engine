#pragma once

namespace Engine::Window::Event {
struct MouseMoved {
  MouseMoved(int x, int y) : x{x}, y{y} {}
  int x, y;
};

struct MouseScrolled {
  MouseScrolled(int x, int y) : x{x}, y{y} {}
  int x, y;
};

struct MouseButtonEvent {
  enum class MouseButton { Left, Middle, Right, Unsupported };
  MouseButtonEvent(MouseButton button, int x, int y)
      : button{button},
        x{x},
        y{y} {}
  MouseButton button;
  int x, y;
};

struct MouseButtonDown : public MouseButtonEvent {
  MouseButtonDown(MouseButton button, int x, int y)
      : MouseButtonEvent{button, x, y} {}
};

struct MouseButtonUp : public MouseButtonEvent {
  MouseButtonUp(MouseButton button, int x, int y)
      : MouseButtonEvent{button, x, y} {}
};
} // namespace Engine::Window::Event