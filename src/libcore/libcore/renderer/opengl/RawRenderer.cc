#include <numeric>

#include <libcore/lib/Math.hh>
#include <libcore/lib/Vector.hh>
#include <libcore/renderer/opengl/Glad.hh>
#include <libcore/renderer/opengl/RawRenderer.hh>
#include <libcore/renderer/opengl/Shader.hh>
#include <libcore/renderer/opengl/Texture.hh>

namespace Engine::Renderer::OpenGL {
RawRenderer::RawRenderer() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RawRenderer::prepareScene() {
  assert(dynamic_cast<OpenGL::Shader*>(_shader.get()));
  glUseProgram(static_cast<OpenGL::Shader*>(_shader.get())->programID());
}

void RawRenderer::beginScene() {
  _target->bind();
  glBindVertexArray(_vertexArray->arrayID());
  glClear(GL_COLOR_BUFFER_BIT);
}

void RawRenderer::endScene() {
  _target->unbind();
  glBindVertexArray(0);
}

void RawRenderer::destroyScene() {
  glUseProgram(0);
}

void RawRenderer::setupBuffers() {
  _vertexArray = createUnique<VertexArray>();
  _vertexArray->setBuffers(*static_cast<VertexBuffer*>(_vertexBuf.get()),
                           *static_cast<IndexBuffer*>(_indexBuf.get()));
}

void RawRenderer::setupShader() {
  Vector<int> samplers{16};
  std::iota(samplers.begin(), samplers.end(), 0);

  assert(dynamic_cast<OpenGL::Shader*>(_shader.get()));
  glUseProgram(static_cast<OpenGL::Shader*>(_shader.get())->programID());

  GLint location = glGetUniformLocation(
      static_cast<Shader*>(_shader.get())->programID(), "u_Textures");
  glUniform1iv(
      location, static_cast<GLsizei>(samplers.size()), samplers.data());
}

void RawRenderer::setClearColor(const Vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void RawRenderer::bindTextures(const Vector<Renderer::Texture*>& textures) {
  for (auto it = textures.begin(); it != textures.end(); it++) {
    glActiveTexture(GL_TEXTURE0 +
                    static_cast<int>(std::distance(textures.begin(), it)));
    glBindTexture(GL_TEXTURE_2D, static_cast<Texture*>(*it)->textureID());
  }
}

void RawRenderer::uploadShaderUniforms(
    const std::initializer_list<UniformType>& uniforms) {
  for (const auto& u : uniforms) {
    // TODO: std::visit
    if (const Mat4 * mat; (mat = std::get_if<Mat4>(&u.value)) != nullptr) {
      _shader->setMat4(u.name, *mat);
    }
  }
}

void RawRenderer::setVertexBufferData(MemBlock data) {
  _vertexBuf->setData(data);
}

void RawRenderer::drawIndexed(uint32_t indexCount) {
  glDrawElements(GL_TRIANGLES,
                 indexCount ? indexCount
                            : static_cast<uint32_t>(_indexBuf->size()),
                 GL_UNSIGNED_INT,
                 nullptr);
}
} // namespace Engine::Renderer::OpenGL