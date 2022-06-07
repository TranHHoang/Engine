#pragma once
#include <any>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <thread>

#include <libcore/ecs/Scene.hh>
#include <libcore/ecs/System.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/lib/Queue.hh>
#include <libcore/renderer/Factory.hh>
#include <libcore/window/Factory.hh>
#include <libcore/window/Props.hh>
#include <libcore/window/events/Event.hh>

namespace Engine::Window {
class Window {
public:
  Window(const Props& props,
         Unique<Factory> windowFactory,
         Unique<Renderer::Factory> rendererFactory,
         std::any data,
         bool singleThread,
         bool waitForWindowInit);

  ECS::Scene& activeScene() { return _activeScene; }

  bool create();
  void mainLoop();
  void startEventLoop();

private:
  void init();
  void destroy();
  std::optional<Event::EventType> handleWindowEvent();
  void handleRenderingEvent();
  void updateSystems();

private:
  Props _props;
  Unique<Factory> _windowFactory;
  Unique<Renderer::Factory> _rendererFactory;
  Unique<Renderer::Renderer> _renderer;
  Queue<Event::EventType> _eventQueue;
  Vector<Unique<ECS::System::System>> _systems;
  Ref<Renderer::Target> _defaultTarget;
  ECS::Scene _activeScene;
  std::atomic<bool> _rendererRunning, _eventLoopRunning;
  bool _singleThread, _waitForWindowInit, _initialized;
  std::thread _renderingThread;
  std::mutex _mutex;
  std::any _data;
  std::condition_variable _condVar;
};
} // namespace Engine::Window