#pragma once
#include <cstdint>

#include <libcore/Resource.hh>
#include <libcore/renderer/Texture.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer::OpenGL {
class Texture : public Engine::Renderer::Texture {
public:
  Texture(ResourceID id, uint32_t width, uint32_t height, const Info& info);

  GLuint textureID() const { return _textureID; }

  void initTexture(const std::byte* data) override;

private:
  GLenum _internalFormat, _dataFormat;
  GLuint _textureID;
};
} // namespace Engine::Renderer::OpenGL