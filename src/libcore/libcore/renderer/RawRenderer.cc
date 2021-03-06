#include <libcore/renderer/RawRenderer.hh>

namespace Engine::Renderer {
void RawRenderer::setData(Unique<VertexBuffer> vertexBuf,
                          Unique<IndexBuffer> indexBuf,
                          Unique<Shader> shader) {
  _vertexBuf = std::move(vertexBuf);
  _indexBuf = std::move(indexBuf);
  setupBuffers();

  _shader = std::move(shader);
  setupShader();
}

void RawRenderer::setTarget(const Ref<Target>& target) {
  _target = target;
}
} // namespace Engine::Renderer