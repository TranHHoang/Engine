#include <glad/gl.h>

#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/Context.hh>

namespace Engine::Renderer {
OpenGLContext::OpenGLContext(const OpenGLPlatformProvider& provider)
    : Context{API::OpenGL},
      _provider{provider} {
  _context = _provider.createContext();
  int version = gladLoaderLoadGL();
  if (version) {
    Logger::info("Loaded OpenGL version {}",
                 reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  } else {
    Logger::error("Unable to load OpenGL library");
  }
}

void OpenGLContext::destroy() {
  _provider.destroyContext(_context);
}
} // namespace Engine::Renderer