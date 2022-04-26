#include <glad/gl.h>

#include <libcore/lib/Assert.hh>
#include <libcore/renderer/opengl/Target.hh>

namespace Engine::Renderer {
OpenGLTarget::OpenGLTarget(const Info& info) : Target{info} {
  invalidate();
}

OpenGLTarget::~OpenGLTarget() {
  glDeleteFramebuffers(1, &_framebufferID);
  glDeleteTextures(1, &_colorAttachmentTexID);
}

void OpenGLTarget::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
  glViewport(0, 0, _info.width, _info.height);
}

void OpenGLTarget::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLTarget::invalidate() {
  if (_framebufferID) { // Delete existing framebuffer
    glDeleteFramebuffers(1, &_framebufferID);
    glDeleteTextures(1, &_colorAttachmentTexID);
  }

  // Create new framebuffer
  glGenFramebuffers(1, &_framebufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

  // Generate color buffer attachments
  glGenTextures(1, &_colorAttachmentTexID);
  glBindTexture(GL_TEXTURE_2D, _colorAttachmentTexID);
  // Generate texture without data (null), we will fill it later
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA8,
               _info.width,
               _info.height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach generated texture to framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         _colorAttachmentTexID,
                         0);

  // Check if framebuffer is completed
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  // Unbind this framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace Engine::Renderer