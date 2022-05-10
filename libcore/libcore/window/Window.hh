#pragma once
#include <atomic>
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
#include <libcore/window/event/Event.hh>

namespace Engine::Window {
class Window {
public:
  Window(const Props& props,
         Unique<Factory> windowFactory,
         Unique<Renderer::Factory> rendererFactory);
  ~Window();

  ECS::Scene& activeScene() { return _activeScene; }

  bool create(bool singleThread = false);
  void mainLoop();
  void startEventLoop();

private:
  void init();
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
  std::atomic<bool> _running;
  std::thread _renderingThread;
  std::mutex _eventQueueMutex;
};
} // namespace Engine::Window