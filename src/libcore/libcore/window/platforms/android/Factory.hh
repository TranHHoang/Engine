#pragma once
#include <any>
#include <optional>

#include <EGL/egl.h>
#include <android_native_app_glue.h>

#include <libcore/lib/Queue.hh>
#include <libcore/window/Factory.hh>

namespace Engine::Window {
class AndroidFactory : public Factory {
public:
  bool createNativeWindow(const Props& props, std::any data) override;
  void showNativeWindow() const override {}
  void destroyNativeWindow() override;
  std::optional<Event::EventType> nextEvent() override;
  Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const override;
  void swapBuffers() override;

  friend void onAppCmd(android_app* app, int32_t cmd);
  friend int32_t onInputEvent(android_app* app, AInputEvent* event);

private:
  bool init();

private:
  Queue<Event::EventType> _eventQueue;
  EGLDisplay _display;
  EGLSurface _surface;
  EGLConfig _config;
  android_app* _app;
};
} // namespace Engine::Window