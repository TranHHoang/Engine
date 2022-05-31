#pragma once
#include <Windows.h>

#include <libcore/lib/Queue.hh>
#include <libcore/window/Factory.hh>

namespace Engine::Window {
class WindowsFactory : public Factory {
public:
  bool createNativeWindow(const Props& props, std::any) override;
  void showNativeWindow() const override;
  std::optional<Event::EventType> nextEvent() override;
  void destroyNativeWindow() override;
  Unique<Renderer::PlatformProvider>
  createPlatformProvider(Renderer::API api) const override;
  void swapBuffers() override;

  friend LRESULT CALLBACK WndProc(HWND hwnd,
                                  UINT msg,
                                  WPARAM wparam,
                                  LPARAM lparam);

private:
  LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
  Props _props;
  Queue<Event::EventType> _eventQueue;
  HWND _hWnd;
};
} // namespace Engine::Window