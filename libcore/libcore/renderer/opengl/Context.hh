#pragma once
#include <any>

#include <libcore/renderer/Context.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>

namespace Engine::Renderer {
class OpenGLContext : public Context {
public:
  OpenGLContext(const OpenGLPlatformProvider& provider);
  void destroy() override;

private:
  OpenGLPlatformProvider _provider;
  std::any _context;
};
} // namespace Engine::Renderer