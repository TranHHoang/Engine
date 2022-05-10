#pragma once
#include <any>

#include <libcore/lib/Queue.hh>
#include <libcore/window/Factory.hh>

namespace Engine::Window {
class EmscriptenFactory : public Factory {
public:
  bool createNativeWindow(const Props& props) override;
  void showNativeWindow() const override {}
  void destroyNativeWindow() override {}
  std::optional<Event::EventType> nextEvent() override;
  Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const override;
  void swapBuffers() override;

  friend int handleEvents(int type, const void* event, void* userData);

private:
  Queue<Event::EventType> _eventQueue;
  std::any _context;
};
} // namespace Engine::Window