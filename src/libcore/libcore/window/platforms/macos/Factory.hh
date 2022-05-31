#pragma once
#include <libcore/lib/Queue.hh>
#include <libcore/window/Factory.hh>
#include <libcore/window/events/Event.hh>

namespace Engine::Window {
class MacOSFactory : public Factory {
public:
  bool createNativeWindow(const Props& props, std::any) override;
  void showNativeWindow() const override;
  void destroyNativeWindow() override;
  Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const override;
  std::optional<Event::EventType> nextEvent() override;
  void swapBuffers() override;

  friend void pushToEventQueue(MacOSFactory* factory, Event::EventType event);

private:
  void *_window, *_app, *_view;
  Queue<Event::EventType> _eventQueue;
};
} // namespace Engine::Window