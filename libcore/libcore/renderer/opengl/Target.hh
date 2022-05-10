#pragma once
#include <libcore/renderer/Target.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer {
class OpenGLTarget : public Target {
public:
  OpenGLTarget(const Info& info);
  ~OpenGLTarget();

  void bind() override;
  void unbind() override;
  void invalidate() override;

private:
  GLuint _framebufferID, _colorAttachmentTexID;
};
} // namespace Engine::Renderer