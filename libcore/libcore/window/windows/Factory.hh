#pragma once
#include <variant>

#include <Windows.h>

#include <libcore/window/Factory.hh>

namespace Engine::Window {
class WindowsFactory : public Factory {
public:
  bool createNativeWindow(const Props& props) override;
  void showNativeWindow() const override;
  Queue<Event::EventType>& eventQueue() override;
  void destroyNativeWindow() override;
  Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const override;
  void swapBuffers() override;

  LRESULT CALLBACK wndProc(HWND hwnd,
                           UINT message,
                           WPARAM wparam,
                           LPARAM lparam);

private:
  Props _props;
  Queue<Event::EventType> _eventQueue;
  HWND _hWnd;
};
} // namespace Engine::Window