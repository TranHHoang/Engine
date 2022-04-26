#include <libcore/renderer/opengl/Buffer.hh>

namespace Engine::Renderer::Buffer {
OpenGLVertex::OpenGLVertex(size_t size, const Layout& layout)
    : Vertex{size, layout} {
  glGenBuffers(1, &_bufID);
  glBindBuffer(GL_ARRAY_BUFFER, _bufID);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertex::~OpenGLVertex() {
  glDeleteBuffers(1, &_bufID);
}

void OpenGLVertex::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, _bufID);
}

void OpenGLVertex::unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertex::setData(size_t size, const void* data) {
  glBindBuffer(GL_ARRAY_BUFFER, _bufID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

OpenGLIndex::OpenGLIndex(size_t size, uint32_t* indices) : Index(size) {
  glGenBuffers(1, &_bufID);
  glBindBuffer(GL_ARRAY_BUFFER, _bufID);
  glBufferData(
      GL_ARRAY_BUFFER, size * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndex::~OpenGLIndex() {
  glDeleteBuffers(1, &_bufID);
}

void OpenGLIndex::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufID);
}

void OpenGLIndex::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
} // namespace Engine::Renderer::Buffer