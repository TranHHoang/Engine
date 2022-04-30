#pragma once
#include <libcore/renderer/Context.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>

namespace Engine::Renderer {
class OpenGLContext : public Context {
public:
  OpenGLContext(const OpenGLPlatformProvider& provider);
  void destroy() override;

private:
  OpenGLPlatformProvider _provider;
  void* _context;
};
} // namespace Engine::Renderer