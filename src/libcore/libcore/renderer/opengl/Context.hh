#pragma once
#include <libcore/renderer/Context.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>

namespace Engine::Renderer::OpenGL {
class Context : public Engine::Renderer::Context {
public:
  Context(const PlatformProvider& provider)
      : Engine::Renderer::Context{API::OpenGL},
        _provider{provider} {}
  ~Context();

  bool init() override;

private:
  PlatformProvider _provider;
  std::any _context;
};
} // namespace Engine::Renderer::OpenGL