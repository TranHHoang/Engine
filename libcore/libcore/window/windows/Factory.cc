#include <any>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>
#include <libcore/window/input/Key.hh>
#include <libcore/window/windows/Factory.hh>

namespace Engine::Window {
static Key toKey(WPARAM key) {
  if (0x30 <= key && key <= 0x39) {
    return static_cast<Key>(static_cast<int>(Key::D0) + (key - 0x30));
  }
  if (0x41 <= key && key <= 0x5A) {
    return static_cast<Key>(static_cast<int>(Key::A) + (key - 0x41));
  }
  if (VK_F1 <= key && key <= VK_F12) {
    return static_cast<Key>(static_cast<int>(Key::F1) + (key - VK_F1));
  }
  if (VK_NUMPAD0 <= key && key <= VK_NUMPAD9) {
    return static_cast<Key>(static_cast<int>(Key::KP0) + (key - VK_NUMPAD0));
  }

  switch (key) {
  case VK_RETURN:
    return Key::Return;
  case VK_ESCAPE:
    return Key::Escape;
  case VK_BACK:
    return Key::Backspace;
  case VK_TAB:
    return Key::Tab;
  case VK_SPACE:
    return Key::Space;
  case VK_OEM_MINUS:
    return Key::Minus;
  case VK_OEM_PLUS:
    return Key::Equals;
  case VK_OEM_4:
    return Key::LeftBracket;
  case VK_OEM_6:
    return Key::RightBracket;
  case VK_OEM_5:
    return Key::Backslash;
  case VK_OEM_1:
    return Key::Semicolon;
  case VK_OEM_7:
    return Key::Apostrophe;
  case VK_OEM_3:
    return Key::Grave;
  case VK_OEM_COMMA:
    return Key::Comma;
  case VK_OEM_PERIOD:
    return Key::Period;
  case VK_OEM_2:
    return Key::Slash;
  case VK_CAPITAL:
    return Key::Capslock;
  case VK_SNAPSHOT:
    return Key::Printscreen;
  case VK_SCROLL:
    return Key::ScrollLock;
  case VK_PAUSE:
    return Key::Pause;
  case VK_INSERT:
    return Key::Insert;
  case VK_HOME:
    return Key::Home;
  case VK_PRIOR:
    return Key::PageUp;
  case VK_DELETE:
    return Key::Delete;
  case VK_END:
    return Key::End;
  case VK_NEXT:
    return Key::PageDown;
  case VK_RIGHT:
    return Key::Right;
  case VK_LEFT:
    return Key::Left;
  case VK_DOWN:
    return Key::Down;
  case VK_UP:
    return Key::Up;
  case VK_DIVIDE:
    return Key::KPDivide;
  case VK_MULTIPLY:
    return Key::KPMultiply;
  case VK_SUBTRACT:
    return Key::KPSubtract;
  case VK_ADD:
    return Key::KPAdd;
  case VK_SEPARATOR:
    return Key::KPPeriod;
  case VK_LCONTROL:
    return Key::LCtrl;
  case VK_LSHIFT:
    return Key::LShift;
  case VK_LMENU:
    return Key::LAlt;
  case VK_LWIN:
    return Key::LGui;
  case VK_RCONTROL:
    return Key::RCtrl;
  case VK_RSHIFT:
    return Key::RShift;
  case VK_RMENU:
    return Key::RAlt;
  case VK_RWIN:
    return Key::RGui;
  }

  assert(false);
  return Key::Null;
}

LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT message,
                         WPARAM wparam,
                         LPARAM lparam) {
  WindowsFactory* pThis;

  if (message == WM_NCCREATE) {
    pThis = static_cast<WindowsFactory*>(
        reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);
    SetLastError(0);
    if (!SetWindowLongPtr(
            hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis))) {
      if (GetLastError() != 0)
        return FALSE;
    }
  } else {
    pThis = reinterpret_cast<WindowsFactory*>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }

  switch (message) {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }

  if (pThis)
    return pThis->wndProc(hwnd, message, wparam, lparam);
  return FALSE;
} // namespace Engine::Window

LRESULT CALLBACK WindowsFactory::wndProc(HWND hwnd,
                                         UINT message,
                                         WPARAM wparam,
                                         LPARAM lparam) {
  using namespace Event;
  EventType type;
  switch (message) {
  case WM_CLOSE:
    type = WindowClosed{};
    break;
  case WM_KEYUP:
    type = KeyUp{toKey(wparam)};
    break;
  case WM_KEYDOWN:
    type = KeyDown{toKey(wparam)};
    break;
  case WM_SIZE:
    type = WindowResized{LOWORD(lparam), HIWORD(lparam)};
    break;
  case WM_ERASEBKGND:
    return TRUE;
  }

  if (!std::holds_alternative<std::monostate>(type)) {
    _eventQueue.push(type);
    return TRUE;
  }

  return DefWindowProc(hwnd, message, wparam, lparam);
}

bool WindowsFactory::createNativeWindow(const Props& props) {
  _props = props;

  WNDCLASS wc{
      .style = CS_HREDRAW | CS_VREDRAW,
      .lpfnWndProc = WndProc,
      .hCursor = LoadCursor(0, IDC_ARROW),
      .hbrBackground = CreateSolidBrush(COLOR_WINDOW + 1),
      .lpszClassName = props.title.c_str(),
  };

  if (!RegisterClass(&wc))
    return false;

  _hWnd = CreateWindow(props.title.c_str(),
                       props.title.c_str(),
                       WS_OVERLAPPEDWINDOW,
                       0,
                       0,
                       props.width,
                       props.height,
                       NULL,
                       NULL,
                       NULL,
                       this);
  return _hWnd != NULL;
}

void WindowsFactory::showNativeWindow() const {
  ShowWindow(_hWnd, SW_SHOWDEFAULT);
  UpdateWindow(_hWnd);
}

Queue<Event::EventType>& WindowsFactory::eventQueue() {
  MSG msg;
  if (GetMessage(&msg, NULL, 0, 0) > 0) {
    DispatchMessage(&msg);
  }

  return _eventQueue;
}

void WindowsFactory::destroyNativeWindow() {
  DestroyWindow(_hWnd);
  HMODULE hModule = GetModuleHandle(0);
  if (!hModule)
    return;
  UnregisterClass(_props.title.c_str(), hModule);
}

Unique<Renderer::PlatformProvider>
WindowsFactory::createPlatformProvider(Renderer::API api) const {
  using namespace Renderer;

  switch (api) {
  case API::OpenGL: {
    auto provider = createUnique<OpenGLPlatformProvider>();
    provider->createContext = [this] {
      PIXELFORMATDESCRIPTOR pfd{
          .nSize = sizeof(PIXELFORMATDESCRIPTOR),
          .nVersion = 1,
          .dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_COMPOSITION |
                     PFD_GENERIC_ACCELERATED | PFD_DRAW_TO_WINDOW |
                     PFD_DOUBLEBUFFER,
          .iPixelType = PFD_TYPE_RGBA,
          .cColorBits = 32,
          .cDepthBits = 24,
          .cStencilBits = 8,
          .iLayerType = PFD_MAIN_PLANE,
      };

      HDC hDC = GetDC(_hWnd);
      int pixelFormat = ChoosePixelFormat(hDC, &pfd);
      SetPixelFormat(hDC, pixelFormat, &pfd);

      HGLRC context = wglCreateContext(hDC);
      wglMakeCurrent(hDC, context);
      ReleaseDC(_hWnd, hDC);

      return context;
    };
    provider->destroyContext = [](std::any context) {
      if (HDC hDC = wglGetCurrentDC()) {
        wglMakeCurrent(hDC, NULL);
      }
      wglDeleteContext(std::any_cast<HGLRC>(context));
    };
    return provider;
  }
  }

  return nullptr;
}

void WindowsFactory::swapBuffers() {
  HDC hDC = GetDC(_hWnd);
  SwapBuffers(hDC);
  ReleaseDC(_hWnd, hDC);
  InvalidateRect(_hWnd, NULL, TRUE);
}
} // namespace Engine::Window