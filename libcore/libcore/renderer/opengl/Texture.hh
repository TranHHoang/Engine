#pragma once
#include <cstdint>

#include <glad/gl.h>

#include <libcore/Resource.hh>
#include <libcore/renderer/Texture.hh>

namespace Engine::Renderer::Texture {
class OpenGLTexture : public Texture {
public:
  OpenGLTexture(ResourceID id,
                uint32_t width,
                uint32_t height,
                const Info& info);
  void initTexture(const void* data) override;

  GLuint textureID() const { return _textureID; }

private:
  GLenum _internalFormat, _dataFormat;
  GLuint _textureID;
};
} // namespace Engine::Renderer::Texture