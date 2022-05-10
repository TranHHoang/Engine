#pragma once
#include <libcore/lib/Memory.hh>
#include <libcore/window/Props.hh>
#include <libcore/window/Window.hh>

namespace Engine {
class App {
public:
  App(const Window::Props& props);
  virtual ~App() = default;
  void start();

protected:
  Unique<Window::Window> _window;
};
} // namespace Engine