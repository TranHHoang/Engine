#include <variant>

#include <libcore/App.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/Factory.hh>
#include <libcore/window/Props.hh>
#include <libcore/window/event/Event.hh>

#ifndef EMSCRIPTEN
#ifdef WINDOWS
#include <libcore/window/platform/windows/Factory.hh>
using WindowFactory = Engine::Window::WindowsFactory;
#elif LINUX
#include <libcore/window/platform/linux/Factory.hh>
using WindowFactory = Engine::Window::LinuxFactory;
#elif MACOS
#include <libcore/window/platform/macos/Factory.hh>
using WindowFactory = Engine::Window::MacOSFactory;
#endif
static inline bool singleThread = false;
#else
#include <emscripten/emscripten.h>
#include <libcore/window/platform/emscripten/Factory.hh>
using WindowFactory = Engine::Window::EmscriptenFactory;
static inline bool singleThread = true;
#endif

namespace Engine {
App::App(const Window::Props& props) {
  Logger::info("Creating window ({})", Renderer::toString(props.api));

  Unique<Renderer::Factory> rendererFactory = nullptr;
  switch (props.api) {
  case Renderer::API::OpenGL:
    rendererFactory = createUnique<Renderer::OpenGLFactory>();
    break;
  default:
    break;
  }

  _window = createUnique<Window::Window>(Window::Props{},
                                         createUnique<WindowFactory>(),
                                         std::move(rendererFactory));
  if (_window->create(singleThread)) {
    Logger::info("Window (title = '{}', width = {}, height = {}) was created "
                 "successfully",
                 props.title,
                 props.width,
                 props.height);
  } else {
    Logger::error("Failed to create window");
  }
}

void App::start() {
#ifdef EMSCRIPTEN
  emscripten_set_main_loop_arg(
      [](void* arg) { static_cast<Window::Window*>(arg)->mainLoop(); },
      _window.get(),
      0,
      1);
#else
  _window->startEventLoop();
#endif
}
} // namespace Engine