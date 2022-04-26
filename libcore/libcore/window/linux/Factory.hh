#pragma once
#include <thread>
#include <variant>

#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <libcore/window/Factory.hh>

namespace Engine::Window {
class LinuxFactory : public Factory {
public:
  bool createNativeWindow(const Props& props) override;
  void showNativeWindow() const override;
  Queue<Event::EventType>& eventQueue() override;
  void destroyNativeWindow() override;
  Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const override;
  void swapBuffers() override;

private:
  Props _props;
  Queue<Event::EventType> _eventQueue;

  ::Window _window;
  Display* _dpy;
  XVisualInfo* _visualInfo;
  Atom _wmDeleteMessage;
  std::thread a;
};
} // namespace Engine::Window