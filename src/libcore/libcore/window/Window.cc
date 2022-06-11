#include <variant>

#include <libcore/ecs/System.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/lib/Utils.hh>
#include <libcore/renderer/Renderer.hh>
#include <libcore/window/Window.hh>

namespace Engine::Window {
Window::Window(const Props& props,
               Unique<Factory> windowFactory,
               Unique<Renderer::Factory> rendererFactory,
               std::any data,
               bool waitForWindowInit)
    : _props{props},
      _windowFactory{std::move(windowFactory)},
      _rendererFactory{std::move(rendererFactory)},
      _rendererRunning{true},
      _eventLoopRunning{true},
      _waitForWindowInit{waitForWindowInit},
      _initialized{false},
      _data{data} {
}

bool Window::create() {
  bool result = _windowFactory->createNativeWindow(_props, _data);
  if (result) {
    _renderingThread = std::thread{[&] {
      if (_waitForWindowInit) {
        std::unique_lock lock{_mutex};
        _condVar.wait(lock, [this] { return _initialized; });
      }

      init();

      if (not _waitForWindowInit) {
        std::scoped_lock lock{_mutex};
        _initialized = true;
        _condVar.notify_one();
      }

      while (_rendererRunning) {
        {
          std::scoped_lock lock{_mutex};
          handleRenderingEvent();
        }
        updateSystems();
      }

      handleRenderingEvent();
    }};

    if (not _waitForWindowInit) {
      std::unique_lock lock{_mutex};
      _condVar.wait(lock, [this] { return _initialized; });
    }

    _windowFactory->showNativeWindow();
  }

  return result;
}

void Window::init() {
  Logger::info("Creating platform provider object for {}",
               toString(_props.api));
  auto provider = _windowFactory->createPlatformProvider(_props.api);

  Logger::info("Initiating {} renderer", toString(_props.api));
  _renderer = createUnique<Renderer::Renderer>(*_rendererFactory, *provider);

  Logger::info("Initiating default rendering target (swapchain)");
  _defaultTarget =
      _rendererFactory->createDefaultTarget({_props.width, _props.height});

  Logger::info("Initiating systems");
  _systems.clear();
  _systems.add(
      createUnique<ECS::System::Rendering>(*_renderer, _defaultTarget));
  _systems.add(createUnique<ECS::System::Spin>());
}

void Window::handleWindowEvent() {
  using namespace Event;
  std::optional<EventType> event = _windowFactory->nextEvent();
  if (event) {
    auto visitor = overload{
        [this](const WindowInitialized&) {
          if (not _rendererRunning) {
            _rendererRunning = true;
            create();
          }
          {
            std::scoped_lock lock{_mutex};
            _initialized = true;
          }
          _condVar.notify_one();
          return true;
        },
        [this](const WindowClosed& e) {
          _eventLoopRunning = e.keepEventLoop;
          _rendererRunning = _initialized = false;
          destroy();
          return true;
        },
        [](const KeyDown& key) {
          Logger::info("KeyDown: {}", static_cast<int>(key.key));
          return true;
        },
        [](const KeyUp& key) {
          Logger::info("KeyUp: {}", static_cast<int>(key.key));
          return true;
        },
        [](const auto&) { return false; },
    };
    if (not std::visit(visitor, event.value())) {
      std::scoped_lock lock{_mutex};
      _eventQueue.push(event.value());
    }
  }
}

void Window::handleRenderingEvent() {
  using namespace Event;

  auto visitor = overload{
      [&](const WindowResized& e) {
        _defaultTarget->resize(e.width, e.height);
        return true;
      },
      [](const auto&) { return false; },
  };

  for (std::optional<Event::EventType> event;
       (event = _eventQueue.peek()) && std::visit(visitor, event.value());
       _eventQueue.pop())
    ;
}

void Window::updateSystems() {
  for (const auto& system : _systems) {
    system->onUpdate(_activeScene, 0);
  }
  _windowFactory->swapBuffers();
}

/// Only call this method if the rendering thread is active
void Window::startEventLoop() {
  assert(_renderingThread.joinable());

  while (_eventLoopRunning) {
    handleWindowEvent();
  }
}

void Window::destroy() {
  if (_renderingThread.joinable())
    _renderingThread.join();
  _renderer.reset();
  _windowFactory->destroyNativeWindow();
}
} // namespace Engine::Window
