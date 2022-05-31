#include <numeric>

#include <libcore/lib/Math.hh>
#include <libcore/lib/Vector.hh>
#include <libcore/renderer/opengl/Glad.hh>
#include <libcore/renderer/opengl/RawRenderer.hh>
#include <libcore/renderer/opengl/Shader.hh>
#include <libcore/renderer/opengl/Texture.hh>

namespace Engine::Renderer {
OpenGLRawRenderer::OpenGLRawRenderer() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRawRenderer::prepareScene() {
  _shader->bind();
}

void OpenGLRawRenderer::beginScene() {
  _target->bind();
  _vertexArray->bind();
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRawRenderer::endScene() {
  _target->unbind();
  _vertexArray->unbind();
}

void OpenGLRawRenderer::destroyScene() {
  _shader->unbind();
}

void OpenGLRawRenderer::setupBuffers() {
  _vertexArray = createUnique<OpenGLVertexArray>();
  _vertexArray->setBuffers(*static_cast<Buffer::Vertex*>(_vertexBuf.get()),
                           *static_cast<Buffer::Index*>(_indexBuf.get()));
}

void OpenGLRawRenderer::setupShader() {
  Vector<int> samplers{16};
  std::iota(samplers.begin(), samplers.end(), 0);

  _shader->bind();
  GLint location = glGetUniformLocation(
      static_cast<Shader::OpenGLShader*>(_shader.get())->programID(),
      "u_Textures");
  glUniform1iv(
      location, static_cast<GLsizei>(samplers.size()), samplers.data());
}

void OpenGLRawRenderer::setClearColor(const Vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRawRenderer::bindTextures(
    const Vector<Texture::Texture*>& textures) {
  for (auto it = textures.begin(); it != textures.end(); it++) {
    glActiveTexture(GL_TEXTURE0 +
                    static_cast<int>(std::distance(textures.begin(), it)));
    glBindTexture(GL_TEXTURE_2D,
                  static_cast<Texture::OpenGLTexture*>(*it)->textureID());
  }
}

void OpenGLRawRenderer::uploadShaderUniforms(
    const std::initializer_list<ShaderUniformType>& uniforms) {
  for (const auto& u : uniforms) {
    // TODO: std::visit
    if (const Mat4 * mat; (mat = std::get_if<Mat4>(&u.value)) != nullptr) {
      _shader->setMat4(u.name, *mat);
    }
  }
}

void OpenGLRawRenderer::setVertexBufferData(MemBlock data) {
  _vertexBuf->setData(data);
}

void OpenGLRawRenderer::drawIndexed(uint32_t indexCount) {
  glDrawElements(GL_TRIANGLES,
                 indexCount ? indexCount
                            : static_cast<uint32_t>(_indexBuf->size()),
                 GL_UNSIGNED_INT,
                 nullptr);
}
} // namespace Engine::Renderer