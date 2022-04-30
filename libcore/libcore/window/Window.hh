#pragma once
#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>

#include <libcore/ecs/Scene.hh>
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
  virtual ~Window() = default;

  bool create();
  void startEventLoop();
  void destroy();

  ECS::Scene& activeScene() { return _activeScene; }
  void setRunning(bool running) { _running = running; }

private:
  Props _props;
  Unique<Factory> _windowFactory;
  Unique<Renderer::Factory> _rendererFactory;
  Queue<Event::EventType> _eventQueue;
  ECS::Scene _activeScene;
  std::thread _renderingThread;
  std::atomic<bool> _running;
  std::mutex _eventQueueMutex;
};
} // namespace Engine::Window