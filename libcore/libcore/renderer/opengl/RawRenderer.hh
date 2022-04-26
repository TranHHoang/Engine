#pragma once
#include <libcore/renderer/RawRenderer.hh>
#include <libcore/renderer/opengl/VertexArray.hh>

namespace Engine::Renderer {
class OpenGLRawRenderer : public RawRenderer {
public:
  OpenGLRawRenderer();

  void prepareScene() override;
  void beginScene() override;
  void endScene() override;
  void destroyScene() override;
  void setClearColor(const Vec4& color) override;
  void bindTextures(std::span<Ref<Texture::Texture>> textures) override;
  void uploadShaderUniforms(
      const std::initializer_list<ShaderUniformType>& uniforms) override;
  void setVertexBufferData(size_t size, const void* data) override;
  void drawIndexed(uint32_t indexCount) override;

private:
  void setupBuffers() override;
  void setupShader() override;

private:
  Unique<OpenGLVertexArray> _vertexArray;
};
} // namespace Engine::Renderer