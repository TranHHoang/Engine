#pragma once
#include <glad/gl.h>

#include <libcore/renderer/Target.hh>

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