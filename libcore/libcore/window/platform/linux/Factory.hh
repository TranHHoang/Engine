#pragma once
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
  Event::EventType nextEvent() override;
  void destroyNativeWindow() override;
  Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const override;
  void swapBuffers() override;

private:
  Props _props;
  ::Window _window;
  Display* _dpy;
  XVisualInfo* _visualInfo;
  Atom _wmDeleteMessage;
};
} // namespace Engine::Window