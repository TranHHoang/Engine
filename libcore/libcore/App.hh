#pragma once
#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Context.hh>
#include <libcore/window/Props.hh>
#include <libcore/window/Window.hh>

namespace Engine {
class App {
public:
  App(const Window::Props& props, Renderer::API api);
  virtual ~App();

  void start();

protected:
  Unique<Window::Window> _window;

private:
  bool _running;
};
} // namespace Engine