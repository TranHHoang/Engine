#pragma once
#include <libcore/renderer/RawRenderer.hh>
#include <libcore/renderer/opengl/VertexArray.hh>

namespace Engine::Renderer::OpenGL {
class RawRenderer : public Engine::Renderer::RawRenderer {
public:
  RawRenderer();
  void prepareScene() override;
  void beginScene() override;
  void endScene() override;
  void destroyScene() override;
  void setClearColor(const Vec4& color) override;
  void bindTextures(const Vector<Texture*>& textures) override;
  void uploadShaderUniforms(
      const std::initializer_list<UniformType>& uniforms) override;
  void setVertexBufferData(MemBlock data) override;
  void drawIndexed(uint32_t indexCount) override;

private:
  void setupBuffers() override;
  void setupShader() override;

private:
  Unique<VertexArray> _vertexArray;
};
} // namespace Engine::Renderer::OpenGL