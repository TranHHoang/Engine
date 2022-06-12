#import <GLKit/GLKit.h>
#import <UIKit/UIKit.h>

#include <libcore/renderer/opengl/Glad.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>
#include <libcore/window/platforms/ios/Factory.hh>

@interface IOSViewController : UIViewController
@end

@implementation IOSViewController
- (BOOL)prefersHomeIndicatorAutoHidden {
  return YES;
}

- (BOOL)prefersStatusBarHidden {
  return YES;
}
@end

namespace Engine::Window {
bool IOSFactory::createNativeWindow(const Props&, std::any data) {
  CGRect rect = [[UIScreen mainScreen] bounds];
  auto view = [[GLKView alloc] initWithFrame:rect];
  _view = view;
  [view resignFirstResponder];
  [view setEnableSetNeedsDisplay:YES];
  [view setHidden:NO];
  [view setOpaque:YES];
  view.backgroundColor = [UIColor clearColor];

  auto viewController = [[IOSViewController alloc] init];
  _viewController = viewController;
  [viewController setView:view];
  [viewController setNeedsUpdateOfHomeIndicatorAutoHidden];
  [viewController setNeedsStatusBarAppearanceUpdate];

  auto window = std::any_cast<void**>(data);
  _window = *window = [[UIWindow alloc] initWithFrame:rect];
  [static_cast<UIWindow*>(*window) setRootViewController:viewController];
  [static_cast<UIWindow*>(*window) makeKeyAndVisible];

  return true;
}

void IOSFactory::destroyNativeWindow() {
  [static_cast<UIWindow*>(_window) release];
  [static_cast<GLKView*>(_view) release];
  [static_cast<IOSViewController*>(_viewController) release];
}

std::optional<Event::EventType> IOSFactory::nextEvent() {
  return {};
  // while (CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.0001, true) ==
  //        kCFRunLoopRunHandledSource)
  //   ;
}

Unique<Renderer::PlatformProvider>
IOSFactory::createPlatformProvider(Renderer::API api) const {
  using namespace Renderer;
  switch (api) {
  case API::OpenGL: {
    auto provider = createUnique<OpenGL::PlatformProvider>();
    provider->createContext = [this] {
      EAGLContext* context =
          [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
      auto view = static_cast<GLKView*>(_view);
      view.context = context;
      [EAGLContext setCurrentContext:context];
      [view bindDrawable];
      return context;
    };
    provider->destroyContext = [](std::any context) {
      [EAGLContext setCurrentContext:nil];
      [std::any_cast<EAGLContext*>(context) release];
    };
    return provider;
  }
  }

  return nullptr;
}

void IOSFactory::swapBuffers() {
  [static_cast<GLKView*>(_view) display];
}
} // namespace Engine::Window
