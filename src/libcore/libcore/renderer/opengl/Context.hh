#pragma once
#include <libcore/renderer/Context.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>

namespace Engine::Renderer {
class OpenGLContext : public Context {
public:
  OpenGLContext(const OpenGLPlatformProvider& provider, API api = API::OpenGL)
      : Context{api},
        _provider{provider} {}
  ~OpenGLContext();

  bool init() override;

private:
  OpenGLPlatformProvider _provider;
  std::any _context;
};
} // namespace Engine::Renderer