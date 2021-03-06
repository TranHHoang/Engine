#pragma once
#include <any>
#include <optional>

#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Context.hh>
#include <libcore/renderer/PlatformProvider.hh>
#include <libcore/window/Props.hh>
#include <libcore/window/events/Event.hh>

namespace Engine::Window {
class Factory {
public:
  virtual ~Factory() = default;

  virtual bool createNativeWindow(const Props& props, std::any data) = 0;
  virtual void showNativeWindow() const = 0;
  virtual std::optional<Event::EventType> nextEvent() = 0;
  virtual void destroyNativeWindow() = 0;
  virtual Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const = 0;
  virtual void swapBuffers() = 0;
};
} // namespace Engine::Window