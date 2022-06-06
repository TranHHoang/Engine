#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/Context.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer {
bool OpenGLContext::init() {
  _context = _provider.createContext();

  int version =
#if defined(EMSCRIPTEN) || defined(ANDROID) || defined(IOS)
      1;
#else
      gladLoaderLoadGL();
#endif

  if (version) {
    Logger::info("Loaded: {}",
                 reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    return true;
  } else {
    Logger::error("Unable to load OpenGL library");
    return false;
  }
}

OpenGLContext::~OpenGLContext() { _provider.destroyContext(_context); }
} // namespace Engine::Renderer