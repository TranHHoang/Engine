#include <libcore/renderer/opengl/Texture.hh>

namespace Engine::Renderer::Texture {
OpenGLTexture::OpenGLTexture(ResourceID id,
                             uint32_t width,
                             uint32_t height,
                             const Info& info)
    : Texture::Texture{id, width, height, info} {
  _internalFormat = GL_RGBA8;
  _dataFormat = GL_RGBA;
}

void OpenGLTexture::initTexture(const std::byte* data) {
  glGenTextures(1, &_textureID);
  glBindTexture(GL_TEXTURE_2D, _textureID);

  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  _info.min == Info::Filter::Linear ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  _info.mag == Info::Filter::Linear ? GL_LINEAR : GL_NEAREST);
  switch (_info.u) {
  case Info::AddressMode::Repeat:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    break;
  }

  switch (_info.v) {
  case Info::AddressMode::Repeat:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    break;
  }

  glTexImage2D(GL_TEXTURE_2D,
               0,
               _internalFormat,
               _width,
               _height,
               0,
               _dataFormat,
               GL_UNSIGNED_BYTE,
               data);
}
} // namespace Engine::Renderer::Texture