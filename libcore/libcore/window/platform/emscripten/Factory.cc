#include <emscripten/em_types.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgl.h>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>
#include <libcore/window/input/Key.hh>
#include <libcore/window/platform/emscripten/Factory.hh>

namespace Engine::Window {
inline constexpr unsigned long hash_djb2a(const std::string_view sv) {
  unsigned long hash{5381};
  for (unsigned char c : sv) {
    hash = ((hash << 5) + hash) ^ c;
  }
  return hash;
}

inline constexpr unsigned long operator""_sh(const char* str, size_t len) {
  return hash_djb2a(std::string_view{str, len});
}

static Key toKey(std::string_view key) {
  switch (hash_djb2a(key)) {
#define NUM_KEY(num)                                                           \
  case hash_djb2a("Digit" #num):                                               \
    return Key::D##num;                                                        \
  case hash_djb2a("Numpad" #num):                                              \
    return Key::KP##num;
    NUM_KEY(0);
    NUM_KEY(1);
    NUM_KEY(2);
    NUM_KEY(3);
    NUM_KEY(4);
    NUM_KEY(5);
    NUM_KEY(6);
    NUM_KEY(7);
    NUM_KEY(8);
    NUM_KEY(9);
#define ALPHA_KEY(alpha)                                                       \
  case hash_djb2a("Key" #alpha):                                               \
    return Key::alpha;
    ALPHA_KEY(A);
    ALPHA_KEY(B);
    ALPHA_KEY(C);
    ALPHA_KEY(D);
    ALPHA_KEY(E);
    ALPHA_KEY(F);
    ALPHA_KEY(G);
    ALPHA_KEY(H);
    ALPHA_KEY(I);
    ALPHA_KEY(J);
    ALPHA_KEY(K);
    ALPHA_KEY(L);
    ALPHA_KEY(M);
    ALPHA_KEY(N);
    ALPHA_KEY(O);
    ALPHA_KEY(P);
    ALPHA_KEY(Q);
    ALPHA_KEY(R);
    ALPHA_KEY(S);
    ALPHA_KEY(T);
    ALPHA_KEY(U);
    ALPHA_KEY(V);
    ALPHA_KEY(W);
    ALPHA_KEY(X);
    ALPHA_KEY(Y);
    ALPHA_KEY(Z);
#define F_KEY(key)                                                             \
  case hash_djb2a("F" #key):                                                   \
    return Key::F##key;
    F_KEY(1);
    F_KEY(2);
    F_KEY(3);
    F_KEY(4);
    F_KEY(5);
    F_KEY(6);
    F_KEY(7);
    F_KEY(8);
    F_KEY(9);
    F_KEY(10);
    F_KEY(11);
    F_KEY(12);
  case "Enter"_sh:
    return Key::Enter;
  case "Escape"_sh:
    return Key::Escape;
  case "Backspace"_sh:
    return Key::Backspace;
  case "Tab"_sh:
    return Key::Tab;
  case "Space"_sh:
    return Key::Space;
  case "Minus"_sh:
    return Key::Minus;
  case "Equal"_sh:
    return Key::Equal;
  case "BracketLeft"_sh:
    return Key::LeftBracket;
  case "BracketRight"_sh:
    return Key::RightBracket;
  case "Backslash"_sh:
    return Key::Backslash;
  case "Semicolon"_sh:
    return Key::Semicolon;
  case "Quote"_sh:
    return Key::Apostrophe;
  case "Backquote"_sh:
    return Key::Grave;
  case "Comma"_sh:
    return Key::Comma;
  case "Period"_sh:
    return Key::Period;
  case "Slash"_sh:
    return Key::Slash;
  case "CapsLock"_sh:
    return Key::Capslock;
  case "PrintScreen"_sh:
    return Key::Printscreen;
  case "ScrollLock"_sh:
    return Key::ScrollLock;
  case "Pause"_sh:
    return Key::Pause;
  case "Insert"_sh:
    return Key::Insert;
  case "Home"_sh:
    return Key::Home;
  case "PageUp"_sh:
    return Key::PageUp;
  case "Delete"_sh:
    return Key::Delete;
  case "End"_sh:
    return Key::End;
  case "PageDown"_sh:
    return Key::PageDown;
  case "ArrowRight"_sh:
    return Key::Right;
  case "ArrowLeft"_sh:
    return Key::Left;
  case "ArrowDown"_sh:
    return Key::Down;
  case "ArrowUp"_sh:
    return Key::Up;
  case "NumpadDivide"_sh:
    return Key::KPDivide;
  case "NumpadMultiply"_sh:
    return Key::KPMultiply;
  case "NumpadSubtract"_sh:
    return Key::KPSubtract;
  case "NumpadAdd"_sh:
    return Key::KPAdd;
  case "NumpadDecimal"_sh:
    return Key::KPPeriod;
  case "ControlLeft"_sh:
    return Key::LCtrl;
  case "ShiftLeft"_sh:
    return Key::LShift;
  case "AltLeft"_sh:
    return Key::LAlt;
  case "OSLeft"_sh:
  case "MetaLeft"_sh:
    return Key::LGui;
  case "ControlRight"_sh:
    return Key::RCtrl;
  case "ShiftRight"_sh:
    return Key::RShift;
  case "AltRight"_sh:
    return Key::RAlt;
  case "OSRight"_sh:
  case "MetaRight"_sh:
    return Key::RGui;
  }

  assert(false);
  return Key::Null;
}

EM_BOOL handleEvents(int type, const void* event, void* userData) {
  std::optional<Event::EventType> result;

  switch (type) {
  case EMSCRIPTEN_EVENT_KEYDOWN:
  case EMSCRIPTEN_EVENT_KEYUP: {
    auto e = static_cast<const EmscriptenKeyboardEvent*>(event);
    if (type == EMSCRIPTEN_EVENT_KEYUP) {
      result = Event::KeyUp{toKey(e->code)};
    } else {
      result = Event::KeyDown{toKey(e->code)};
    }
    break;
  }
  case EMSCRIPTEN_EVENT_RESIZE: {
    auto e = static_cast<const EmscriptenUiEvent*>(event);
    result = Event::WindowResized{static_cast<uint32_t>(e->windowInnerWidth),
                                  static_cast<uint32_t>(e->windowInnerHeight)};
    break;
  }
  }
  auto factory = static_cast<EmscriptenFactory*>(userData);
  factory->_eventQueue.push(result.value());

  return EM_TRUE;
}

bool EmscriptenFactory::createNativeWindow(const Props& props) {
  const char* target = "#canvas";
  emscripten_set_canvas_element_size(target, props.width, props.height);
  emscripten_set_window_title(props.title.data());

  auto handleEventFn = [](int type, const auto* event, void* data) -> EM_BOOL {
    return handleEvents(type, reinterpret_cast<const void*>(event), data);
  };

  emscripten_set_keydown_callback(target, this, EM_FALSE, handleEventFn);
  emscripten_set_keyup_callback(target, this, EM_FALSE, handleEventFn);
  emscripten_set_resize_callback(target, this, EM_FALSE, handleEventFn);

  return true;
}

std::optional<Event::EventType> EmscriptenFactory::nextEvent() {
  return _eventQueue.pop();
}

Unique<Renderer::PlatformProvider>
EmscriptenFactory::createPlatformProvider(Renderer::API api) const {
  using namespace Renderer;

  if (api == API::OpenGL) {
    auto provider = createUnique<OpenGLPlatformProvider>();
    provider->createContext = [] {
      EmscriptenWebGLContextAttributes attribs{
          .premultipliedAlpha = false,
          .majorVersion = 2,
          .minorVersion = 0,
      };
      auto context = emscripten_webgl_create_context("#canvas", &attribs);
      emscripten_webgl_make_context_current(context);
      return context;
    };
    provider->destroyContext = [](std::any context) {
      emscripten_webgl_destroy_context(
          std::any_cast<EMSCRIPTEN_WEBGL_CONTEXT_HANDLE>(context));
    };
    return provider;
  }

  return nullptr;
}

void EmscriptenFactory::swapBuffers() {
  emscripten_webgl_commit_frame();
}
} // namespace Engine::Window