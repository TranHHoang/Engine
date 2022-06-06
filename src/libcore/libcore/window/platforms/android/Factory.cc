#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/input.h>
#include <android/looper.h>
#include <jni.h>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/ListUtils.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/lib/Utils.hh>
#include <libcore/lib/Vector.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>
#include <libcore/window/input/Key.hh>
#include <libcore/window/platforms/android/Factory.hh>

namespace Engine::Window {
void onAppCmd(android_app* app, int32_t cmd) {
  std::optional<Event::EventType> event;
  auto factory = static_cast<AndroidFactory*>(app->userData);
  switch (cmd) {
  case APP_CMD_INIT_WINDOW:
    if (app->window && factory->init()) {
      // if (app->savedState) {
      //   factory->_eventQueue.push(Event::WindowRestoreState{
      //       {byte_cast(app->savedState), app->savedStateSize}});
      // }
      event = Event::WindowInitialized{};
    }
    break;
  case APP_CMD_TERM_WINDOW:
    event = Event::WindowClosed{true};
    break;
  case APP_CMD_SAVE_STATE:
    // event = Event::WindowSaveState{&app->savedState, &app->savedStateSize};
    break;
  }

  if (event) {
    factory->_eventQueue.push(event.value());
  }
}

int32_t onInputEvent(android_app* app, AInputEvent* event) {
  return 0;
}

bool AndroidFactory::init() {
  const EGLint attribs[] = {EGL_SURFACE_TYPE,
                            EGL_WINDOW_BIT,
                            EGL_BLUE_SIZE,
                            8,
                            EGL_GREEN_SIZE,
                            8,
                            EGL_RED_SIZE,
                            8,
                            EGL_NONE};
  _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  eglInitialize(_display, nullptr, nullptr);

  int numConfigs;
  eglChooseConfig(_display, attribs, nullptr, 0, &numConfigs);
  Vector<EGLConfig> supportedConfigs{static_cast<size_t>(numConfigs)};
  eglChooseConfig(
      _display, attribs, supportedConfigs.data(), numConfigs, &numConfigs);
  assert(supportedConfigs.size());

  auto config = ListUtils::find(supportedConfigs, [this](const auto& cfg) {
    EGLint r, g, b, d;
    return (eglGetConfigAttrib(_display, cfg, EGL_RED_SIZE, &r) &&
            eglGetConfigAttrib(_display, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(_display, cfg, EGL_BLUE_SIZE, &b) &&
            eglGetConfigAttrib(_display, cfg, EGL_DEPTH_SIZE, &d) && r == 8 &&
            g == 8 && b == 8 && d == 0);
  });
  _config = config.value_or(supportedConfigs[0]);

  if (_config) {
    EGLint format;
    eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);
    _surface = eglCreateWindowSurface(_display, _config, _app->window, nullptr);
  } else {
    Logger::error("Unable to initialize EGLConfig");
  }

  return not null(_config);
}

bool AndroidFactory::createNativeWindow(const Props& props, std::any data) {
  assert(data.has_value());
  _app = std::any_cast<android_app*>(data);
  _app->userData = this;
  _app->onAppCmd = onAppCmd;
  _app->onInputEvent = onInputEvent;
  return true;
}

void AndroidFactory::destroyNativeWindow() {
  eglDestroySurface(_display, _surface);
  eglTerminate(_display);
}

std::optional<Event::EventType> AndroidFactory::nextEvent() {
  int indent;
  int events;
  struct android_poll_source* source;
  while ((indent = ALooper_pollAll(
              0, nullptr, &events, reinterpret_cast<void**>(&source))) >= 0) {
    if (not null(source)) {
      source->process(_app, source);
    }

    if (_app->destroyRequested != 0) {
      return Event::WindowClosed{};
    }

    if (auto event = _eventQueue.pop()) {
      return event.value();
    }
  }

  return {};
}

Unique<Renderer::PlatformProvider>
AndroidFactory::createPlatformProvider(Renderer::API api) const {
  using namespace Renderer;

  if (api == API::OpenGL) {
    auto provider = createUnique<OpenGLPlatformProvider>();
    provider->createContext = [this] {
      EGLint const static contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION,
                                              3, // new requirement
                                              EGL_NONE};
      EGLContext context =
          eglCreateContext(_display, _config, nullptr, contextAttribs);
      if (eglMakeCurrent(_display, _surface, _surface, context) == EGL_FALSE) {
        Logger::error("Unable to eglMakeCurrent");
      }
      return context;
    };
    provider->destroyContext = [this](std::any context) {
      eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
      eglDestroyContext(_display, std::any_cast<EGLContext>(context));
    };
    return provider;
  }

  return nullptr;
}

void AndroidFactory::swapBuffers() {
  eglSwapBuffers(_display, _surface);
}
} // namespace Engine::Window