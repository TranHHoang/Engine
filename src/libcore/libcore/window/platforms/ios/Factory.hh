#pragma once
#include <libcore/window/Factory.hh>

namespace Engine::Window {
class IOSFactory : public Factory {
public:
  bool createNativeWindow(const Props& props, std::any) override;
  void showNativeWindow() const override {}
  void destroyNativeWindow() override;
  Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const override;
  std::optional<Event::EventType> nextEvent() override;
  void swapBuffers() override;

private:
  void *_view, *_viewController, *_window;
};
} // namespace Engine::Window