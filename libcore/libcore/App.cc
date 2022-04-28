#include <libcore/App.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/renderer/opengl/Factory.hh>
#include <libcore/window/Props.hh>
#include <libcore/window/event/Event.hh>

#ifdef WINDOWS
#include <libcore/window/windows/Factory.hh>
using WindowFactory = Engine::Window::WindowsFactory;
#elif LINUX
#include <libcore/window/linux/Factory.hh>
using WindowFactory = Engine::Window::LinuxFactory;
#endif

namespace Engine {
App::App(const Window::Props& props, Renderer::API api) : _running{true} {
  Logger::info("Creating window ({})", Renderer::toString(api));

  Unique<Renderer::Factory> rendererFactory;
  switch (api) {
  case Renderer::API::OpenGL:
    rendererFactory = createUnique<Renderer::OpenGLFactory>();
    break;
  default:
    break;
  }

  _window = createUnique<Window::Window>(Window::Props{},
                                         createUnique<WindowFactory>(),
                                         std::move(rendererFactory));
  if (_window->create(api)) {
    Logger::info("Window (title = '{}', width = {}, height = {}) was created "
                 "successfully",
                 props.title,
                 props.width,
                 props.height);
  } else {
    Logger::error("Failed to create window");
  }
}

App::~App() {
  _window->destroy();
}

void App::start() {
  _window->startEventLoop();
}
} // namespace Engine