#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/XKBlib.h>
#include <fmt/core.h>
#include <sys/select.h>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>
#include <libcore/window/input/Key.hh>
#include <libcore/window/platforms/linux/Factory.hh>

namespace Engine::Window {
static Key toKey(KeySym key) {
  if (XK_0 <= key && key <= XK_9) {
    return static_cast<Key>(static_cast<int>(Key::D0) + (key - XK_0));
  }
  if (XK_A <= key && key <= XK_Z) {
    return static_cast<Key>(static_cast<int>(Key::A) + (key - XK_A));
  }
  if (XK_a <= key && key <= XK_z) {
    return static_cast<Key>(static_cast<int>(Key::A) + (key - XK_a));
  }
  if (XK_F1 <= key && key <= XK_F12) {
    return static_cast<Key>(static_cast<int>(Key::F1) + (key - XK_F1));
  }
  if (XK_KP_0 <= key && key <= XK_KP_9) {
    return static_cast<Key>(static_cast<int>(Key::KP0) + (key - XK_KP_0));
  }

  switch (key) {
  case XK_Return:
    return Key::Enter;
  case XK_Escape:
    return Key::Escape;
  case XK_BackSpace:
    return Key::Backspace;
  case XK_Tab:
    return Key::Tab;
  case XK_space:
    return Key::Space;
  case XK_minus:
    return Key::Minus;
  case XK_plus:
    return Key::Equal;
  case XK_bracketleft:
    return Key::LeftBracket;
  case XK_bracketright:
    return Key::RightBracket;
  case XK_backslash:
    return Key::Backslash;
  case XK_semicolon:
    return Key::Semicolon;
  case XK_apostrophe:
    return Key::Apostrophe;
  case XK_grave:
    return Key::Grave;
  case XK_comma:
    return Key::Comma;
  case XK_period:
    return Key::Period;
  case XK_slash:
    return Key::Slash;
  case XK_Caps_Lock:
    return Key::Capslock;
  case XK_Print:
    return Key::Printscreen;
  case XK_Scroll_Lock:
    return Key::ScrollLock;
  case XK_Pause:
    return Key::Pause;
  case XK_Insert:
    return Key::Insert;
  case XK_Home:
    return Key::Home;
  case XK_Page_Up:
    return Key::PageUp;
  case XK_Delete:
    return Key::Delete;
  case XK_End:
    return Key::End;
  case XK_Page_Down:
    return Key::PageDown;
  case XK_Right:
    return Key::Right;
  case XK_Left:
    return Key::Left;
  case XK_Down:
    return Key::Down;
  case XK_Up:
    return Key::Up;
  case XK_KP_Divide:
    return Key::KPDivide;
  case XK_KP_Multiply:
    return Key::KPMultiply;
  case XK_KP_Subtract:
    return Key::KPSubtract;
  case XK_KP_Add:
    return Key::KPAdd;
  case XK_KP_Separator:
    return Key::KPPeriod;
  case XK_Control_L:
    return Key::LCtrl;
  case XK_Shift_L:
    return Key::LShift;
  case XK_Alt_L:
    return Key::LAlt;
  case XK_Super_L:
    return Key::LGui;
  case XK_Control_R:
    return Key::RCtrl;
  case XK_Shift_R:
    return Key::RShift;
  case XK_Alt_R:
    return Key::RAlt;
  case XK_Super_R:
    return Key::RGui;
  }

  // assert(false);
  return Key::Null;
}

bool LinuxFactory::createNativeWindow(const Props& props, std::any) {
  _props = props;

  XInitThreads();
  _dpy = XOpenDisplay(NULL);
  if (_dpy == NULL) {
    assert(false);
    Logger::error("Fail to connect to X server");
    return false;
  }

  ::Window root = DefaultRootWindow(_dpy);
  _visualInfo = glXChooseVisual(_dpy,
                                0,
                                std::array{GLX_RGBA,
                                           GLX_DEPTH_SIZE,
                                           24,
                                           GLX_DOUBLEBUFFER,
                                           static_cast<int>(None)}
                                    .data());
  if (_visualInfo == NULL) {
    assert(false);
    Logger::error("No appropriate visual found");
    return false;
  } else {
    Logger::info("Visual selected {}",
                 reinterpret_cast<void*>(_visualInfo->visualid));
  }

  Colormap colormap =
      XCreateColormap(_dpy, root, _visualInfo->visual, AllocNone);
  XSetWindowAttributes swa;
  swa.event_mask =
      ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask,
  swa.colormap = colormap;

  _window = XCreateWindow(_dpy,
                          root,
                          0,
                          0,
                          _props.width,
                          _props.height,
                          0,
                          _visualInfo->depth,
                          InputOutput,
                          _visualInfo->visual,
                          CWColormap | CWEventMask,
                          &swa);
  // Listen to window closed event
  _wmDeleteMessage = XInternAtom(_dpy, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(_dpy, _window, &_wmDeleteMessage, 1);

  return true;
}

void LinuxFactory::showNativeWindow() const {
  XMapWindow(_dpy, _window);
  XStoreName(_dpy, _window, _props.title.data());
}

std::optional<Event::EventType> LinuxFactory::nextEvent() {
  using namespace Event;

  int x11fd = ConnectionNumber(_dpy);

  while (true) {
    XEvent xEvent;
    auto pred = [](auto...) { return 1; };

    while (not XCheckIfEvent(_dpy, &xEvent, pred, NULL)) {
      XFlush(_dpy);

      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(x11fd, &fds);

      select(x11fd + 1, &fds, NULL, NULL, NULL);
    }

    std::optional<EventType> event;
    switch (xEvent.type) {
    case ClientMessage:
      if (static_cast<Atom>(xEvent.xclient.data.l[0]) == _wmDeleteMessage)
        event = WindowClosed{};
      break;
    case KeyRelease:
      event = KeyUp{toKey(XkbKeycodeToKeysym(_dpy, xEvent.xkey.keycode, 0, 0))};
      break;
    case KeyPress:
      event =
          KeyDown{toKey(XkbKeycodeToKeysym(_dpy, xEvent.xkey.keycode, 0, 0))};
      break;
    case ConfigureNotify:
      event = WindowResized{static_cast<uint32_t>(xEvent.xconfigure.width),
                            static_cast<uint32_t>(xEvent.xconfigure.height)};
      break;
    }

    if (event.has_value()) {
      return event.value();
    }
  }
}

void LinuxFactory::destroyNativeWindow() {
  XDestroyWindow(_dpy, _window);
  XCloseDisplay(_dpy);
}

Unique<Renderer::PlatformProvider>
LinuxFactory::createPlatformProvider(Renderer::API api) const {
  using namespace Renderer;

  switch (api) {
  case API::OpenGL: {
    auto provider = createUnique<OpenGLPlatformProvider>();
    provider->createContext = [this] {
      GLXContext context = glXCreateContext(_dpy, _visualInfo, NULL, GL_TRUE);
      glXMakeCurrent(_dpy, _window, context);
      return context;
    };
    provider->destroyContext = [this](std::any context) {
      glXMakeCurrent(_dpy, None, NULL);
      glXDestroyContext(_dpy, std::any_cast<GLXContext>(context));
    };
    return provider;
  }
  }

  return nullptr;
}

void LinuxFactory::swapBuffers() {
  glXSwapBuffers(_dpy, _window);
}
} // namespace Engine::Window