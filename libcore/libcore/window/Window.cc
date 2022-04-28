#include <condition_variable>
#include <optional>
#include <variant>

#include <libcore/ecs/System.hh>
#include <libcore/lib/Utils.hh>
#include <libcore/renderer/Renderer.hh>
#include <libcore/window/Window.hh>

namespace Engine::Window {
Window::Window(const Props& props,
               Unique<Factory> windowFactory,
               Unique<Renderer::Factory> rendererFactory)
    : _props{props},
      _windowFactory{std::move(windowFactory)},
      _rendererFactory{std::move(rendererFactory)},
      _running{true} {
}

bool Window::create(Renderer::API api) {
  bool result = _windowFactory->createNativeWindow(_props);
  if (result) {
    std::condition_variable cv;
    bool initDone = false;
    std::mutex mutex;

    _renderingThread = std::thread{[&] {
      Logger::info("Creating platform provider object for {}", toString(api));
      auto provider = _windowFactory->createPlatformProvider(api);

      Logger::info("Initiating {} renderer", toString(api));
      auto renderer =
          createUnique<Renderer::Renderer>(*_rendererFactory, *provider);

      Logger::info("Initiating default rendering target (swapchain)",
                   toString(api));
      auto defaultTarget =
          _rendererFactory->createDefaultTarget({_props.width, _props.height});

      Logger::info("Initiating systems", toString(api));
      Vector<Unique<ECS::System::System>> systems;
      systems.add(
          createUnique<ECS::System::Rendering>(*renderer, defaultTarget));
      systems.add(createUnique<ECS::System::Spin>());

      {
        std::scoped_lock lock{_eventQueueMutex};
        initDone = true;
      }
      cv.notify_one();

      while (_running) {
        using namespace Event;
        {
          std::scoped_lock lock{_eventQueueMutex};
          auto visitor = overload{
              [&](const WindowResized& e) {
                defaultTarget->resize(e.width, e.height);
                return true;
              },
              [](const auto&) { return false; },
          };

          for (std::optional<Event::EventType> event;
               (event = _eventQueue.peek()) &&
               std::visit(visitor, event.value());
               _eventQueue.pop())
            ;
        }

        for (const auto& system : systems) {
          system->onUpdate(_activeScene, 0);
        }
        _windowFactory->swapBuffers();
      }

      renderer->destroy();
    }};

    // Blocking until the initialization is done
    {
      std::unique_lock lock{_eventQueueMutex};
      cv.wait(lock, [&] { return initDone; });
    }

    _windowFactory->showNativeWindow();
  }

  return result;
}

void Window::startEventLoop() {
  using namespace Event;

  while (_running) {
    EventType event = _windowFactory->nextEvent();
    auto visitor = overload{
        [this](const WindowClosed&) {
          _running = false;
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
    if (not std::visit(visitor, event)) {
      std::scoped_lock lock{_eventQueueMutex};
      _eventQueue.push(event);
    }
  }
}

void Window::destroy() {
  _renderingThread.join();
  _windowFactory->destroyNativeWindow();
}
} // namespace Engine::Window