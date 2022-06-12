#include <libcore/renderer/opengl/Buffer.hh>

namespace Engine::Renderer::OpenGL {
VertexBuffer::VertexBuffer(size_t size, const BufferLayout& layout)
    : Renderer::VertexBuffer{size, layout} {
  glGenBuffers(1, &_bufID);
  glBindBuffer(GL_ARRAY_BUFFER, _bufID);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &_bufID);
}

void VertexBuffer::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, _bufID);
}

void VertexBuffer::unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(MemBlock data) {
  glBindBuffer(GL_ARRAY_BUFFER, _bufID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, data.size(), data.data());
}

IndexBuffer::IndexBuffer(std::span<const uint32_t> indices)
    : Renderer::IndexBuffer{indices.size()} {
  glGenBuffers(1, &_bufID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * sizeof(uint32_t),
               indices.data(),
               GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &_bufID);
}

void IndexBuffer::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufID);
}

void IndexBuffer::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
} // namespace Engine::Renderer::OpenGL