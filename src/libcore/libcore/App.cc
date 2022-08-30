#include <libcore/App.hh>
#include <libcore/Platform.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/Factory.hh>
#include <libcore/window/Props.hh>
#include <libcore/window/events/Event.hh>

namespace Engine {
App::App(const Window::Props& props, std::any data) {
  using namespace Renderer;

  Logger::info("Creating window ({})", toString(props.api));

  Unique<Factory> rendererFactory = nullptr;
  switch (props.api) {
  case API::OpenGL:
    rendererFactory = createUnique<OpenGL::Factory>();
    break;
  default:
    break;
  }

  _window = createUnique<Window::Window>(Window::Props{},
                                         createUnique<WindowFactory>(),
                                         std::move(rendererFactory),
                                         data,
                                         WAIT_FOR_WINDOW_INIT);
  if (_window->create()) {
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
      [](void* arg) { static_cast<Window::Window*>(arg)->handleWindowEvent(); },
      _window.get(),
      0,
      1);
#else
  _window->startEventLoop();
#endif
}
} // namespace Engine