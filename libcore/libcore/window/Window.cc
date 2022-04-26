#include <condition_variable>
#include <mutex>

#include <libcore/ecs/System.hh>
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
      _defaultTarget =
          _rendererFactory->createDefaultTarget({_props.width, _props.height});

      Logger::info("Initiating systems", toString(api));
      Vector<Unique<ECS::System::System>> systems;
      systems.add(
          createUnique<ECS::System::Rendering>(*renderer, _defaultTarget));
      systems.add(createUnique<ECS::System::Spin>());

      {
        std::scoped_lock lock{mutex};
        initDone = true;
        cv.notify_one();
      }

      while (_running) {
        for (const auto& system : systems) {
          system->onUpdate(_activeScene, 0);
        }
        _windowFactory->swapBuffers();
      }

      renderer->destroy();
    }};

    // Blocking until the initialization is done
    {
      std::unique_lock lock{mutex};
      cv.wait(lock, [&] { return initDone; });
    }

    _windowFactory->showNativeWindow();
  }

  return result;
}

void Window::resize(uint32_t width, uint32_t height) {
  _defaultTarget->resize(width, height);
}

void Window::destroy() {
  _renderingThread.join();
  _windowFactory->destroyNativeWindow();
}
} // namespace Engine::Window