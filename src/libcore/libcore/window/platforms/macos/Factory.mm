#include <optional>

#define GL_SILENCE_DEPRECATION
#include <Carbon/Carbon.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAOpenGLLayer.h>

#include <libcore/lib/Logger.hh>
#include <libcore/renderer/Context.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>
#include <libcore/window/platforms/macos/Factory.hh>

@interface MacOSWindow : NSWindow <NSApplicationDelegate>
@end

@implementation MacOSWindow
- (void)applicationDidFinishLaunching:(NSNotification*)notification {
  [NSApp stop:nil];
}
@end

@interface WindowEventController : NSObject <NSWindowDelegate> {
  Engine::Window::MacOSFactory* _factory;
}
@end

@implementation WindowEventController
- (instancetype)initWithFactory:(Engine::Window::MacOSFactory*)factory {
  self = [super init];
  if (self != nil)
    _factory = factory;
  return self;
}

- (void)postEmptyEvent {
  @autoreleasepool {
    NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                        location:NSMakePoint(0, 0)
                                   modifierFlags:0
                                       timestamp:0.0
                                    windowNumber:0
                                         context:nil
                                         subtype:0
                                           data1:0
                                           data2:0];
    [NSApp postEvent:event atStart:YES];
  }
}

- (void)windowDidResize:(NSNotification*)notification {
  auto wnd = static_cast<MacOSWindow*>([notification object]);
  const auto contentRect = [[wnd contentView] frame];
  const auto fbRect = [[wnd contentView] convertRectToBacking:contentRect];
  using namespace Engine::Window;
  pushToEventQueue(
      _factory,
      Event::WindowResized{static_cast<uint32_t>(fbRect.size.width),
                           static_cast<uint32_t>(fbRect.size.height)});
  [self postEmptyEvent];
}

- (BOOL)windowShouldClose:(NSWindow*)sender {
  using namespace Engine::Window;
  pushToEventQueue(_factory, Event::WindowClosed{});
  [self postEmptyEvent];

  return NO;
}
@end

namespace Engine::Window {
void pushToEventQueue(MacOSFactory* factory, Event::EventType event) {
  factory->_eventQueue.push(event);
}

bool MacOSFactory::createNativeWindow(const Props& props, std::any) {
  _app = [NSApplication sharedApplication];
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

  NSRect winRect = NSMakeRect(0, 0, props.width, props.height);
  NSWindowStyleMask style =
      NSWindowStyleMaskTitled | NSWindowStyleMaskResizable |
      NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
  _window = [[MacOSWindow alloc] initWithContentRect:winRect
                                           styleMask:style
                                             backing:NSBackingStoreBuffered
                                               defer:YES];
  auto wnd = static_cast<MacOSWindow*>(_window);
  auto title = [NSString stringWithCString:props.title.data()
                                  encoding:[NSString defaultCStringEncoding]];
  [wnd setTitle:title];
  [wnd makeKeyAndOrderFront:wnd];
  [wnd makeKeyWindow];
  [wnd setOpaque:YES];
  [wnd setDelegate:[[WindowEventController alloc] initWithFactory:this]];
  [static_cast<NSApplication*>(_app) setDelegate:wnd];

  switch (props.api) {
  case Renderer::API::OpenGL: {
    NSOpenGLPixelFormatAttribute pixelFormatAttributes[]{
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize,
        24,
        NSOpenGLPFAAlphaSize,
        8,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
        0,
    };
    NSOpenGLPixelFormat* pixelFormat = [[[NSOpenGLPixelFormat alloc]
        initWithAttributes:pixelFormatAttributes] autorelease];
    NSOpenGLView* view =
        [[NSOpenGLView alloc] initWithFrame:[[wnd contentView] bounds]
                                pixelFormat:pixelFormat];
    [wnd setContentView:view];
    [view prepareOpenGL];
    _view = view;
    break;
  }
  }

  return true;
}

void MacOSFactory::showNativeWindow() const {
  [static_cast<NSApplication*>(_app) run];
}

void MacOSFactory::destroyNativeWindow() {
  [static_cast<MacOSWindow*>(_window) release];
}

Unique<Renderer::PlatformProvider>
MacOSFactory::createPlatformProvider(Renderer::API api) const {
  using namespace Renderer;

  switch (api) {
  case API::OpenGL: {
    auto provider = createUnique<OpenGLPlatformProvider>();
    provider->createContext = [this] {
      [[static_cast<NSOpenGLView*>(_view) openGLContext] makeCurrentContext];
      return _view;
    };
    provider->destroyContext = [](std::any context) {
      [std::any_cast<NSOpenGLView*>(context) release];
    };

    return provider;
  }
  }

  return nullptr;
}

Key toKey(unsigned short key) {
  switch (key) {
#define NUM_KEY(num)                                                           \
  case kVK_ANSI_##num:                                                         \
    return Key::D##num;                                                        \
  case kVK_ANSI_Keypad##num:                                                   \
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
  case kVK_ANSI_##alpha:                                                       \
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
  case kVK_F##key:                                                             \
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
  case kVK_Return:
    return Key::Enter;
  case kVK_Escape:
    return Key::Escape;
  case kVK_Delete:
    return Key::Backspace;
  case kVK_Tab:
    return Key::Tab;
  case kVK_Space:
    return Key::Space;
  case kVK_ANSI_Minus:
    return Key::Minus;
  case kVK_ANSI_Equal:
    return Key::Equal;
  case kVK_ANSI_LeftBracket:
    return Key::LeftBracket;
  case kVK_ANSI_RightBracket:
    return Key::RightBracket;
  case kVK_ANSI_Backslash:
    return Key::Backslash;
  case kVK_ANSI_Semicolon:
    return Key::Semicolon;
  case kVK_ANSI_Quote:
    return Key::Apostrophe;
  case kVK_ANSI_Grave:
    return Key::Grave;
  case kVK_ANSI_Comma:
    return Key::Comma;
  case kVK_ANSI_Period:
    return Key::Period;
  case kVK_ANSI_Slash:
    return Key::Slash;
  case kVK_CapsLock:
    return Key::Capslock;
  // case VK_SNAPSHOT:
  //   return Key::Printscreen;
  // case VK_SCROLL:
  //   return Key::ScrollLock;
  // case VK_PAUSE:
  //   return Key::Pause;
  case kVK_Help:
    return Key::Insert;
  case kVK_Home:
    return Key::Home;
  case kVK_PageUp:
    return Key::PageUp;
  case kVK_ForwardDelete:
    return Key::Delete;
  case kVK_End:
    return Key::End;
  case kVK_PageDown:
    return Key::PageDown;
  case kVK_RightArrow:
    return Key::Right;
  case kVK_LeftArrow:
    return Key::Left;
  case kVK_DownArrow:
    return Key::Down;
  case kVK_UpArrow:
    return Key::Up;
  case kVK_ANSI_KeypadDivide:
    return Key::KPDivide;
  case kVK_ANSI_KeypadMultiply:
    return Key::KPMultiply;
  case kVK_ANSI_KeypadMinus:
    return Key::KPSubtract;
  case kVK_ANSI_KeypadPlus:
    return Key::KPAdd;
  case kVK_ANSI_KeypadDecimal:
    return Key::KPPeriod;
  case kVK_Command:
    return Key::LCtrl;
  case kVK_Shift:
    return Key::LShift;
  case kVK_Option:
    return Key::LAlt;
  // case kVK_Op:
  // return Key::LGui;
  case kVK_RightCommand:
    return Key::RCtrl;
  case kVK_RightShift:
    return Key::RShift;
  case kVK_RightOption:
    return Key::RAlt;
    // case VK_RWIN:
    //   return Key::RGui;
  }

  Logger::error("Unknown keycode {}", key);
  assert(false);
  return Key::Null;
}

std::optional<Event::EventType> MacOSFactory::nextEvent() {
  while (true) {
    NSEvent* nsEvent = [NSApp nextEventMatchingMask:NSEventMaskAny
                                          untilDate:[NSDate distantFuture]
                                             inMode:NSDefaultRunLoopMode
                                            dequeue:YES];
    if (nsEvent) {
      using namespace Event;
      std::optional<EventType> event;

      switch ([nsEvent type]) {
      case NSEventTypeKeyDown:
        event = KeyDown{toKey([nsEvent keyCode])};
        break;
      case NSEventTypeKeyUp:
        event = KeyUp{toKey([nsEvent keyCode])};
        break;
      default:
        break;
      }

      if (event) {
        _eventQueue.push(event.value());
      }
      if (auto event = _eventQueue.pop()) {
        return event.value();
      }
      if (nsEvent == nil)
        break;

      [NSApp sendEvent:nsEvent];
    }
  }
}

void MacOSFactory::swapBuffers() {
  auto wnd = static_cast<MacOSWindow*>(_window);
  auto glView = static_cast<NSOpenGLView*>([wnd contentView]);
  [[glView openGLContext] flushBuffer];
}
} // namespace Engine::Window