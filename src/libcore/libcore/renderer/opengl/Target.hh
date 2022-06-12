#pragma once
#include <libcore/renderer/Target.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer::OpenGL {
class Target : public Engine::Renderer::Target {
public:
  Target(const Info& info);
  ~Target();

  void bind() override;
  void unbind() override;
  void invalidate() override;

private:
  GLuint _framebufferID, _colorAttachmentTexID;
};
} // namespace Engine::Renderer::OpenGL