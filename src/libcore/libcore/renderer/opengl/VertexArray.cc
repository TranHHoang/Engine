#include <libcore/renderer/BufferLayout.hh>
#include <libcore/renderer/opengl/VertexArray.hh>

namespace Engine::Renderer::OpenGL {
VertexArray::VertexArray() {
  glGenVertexArrays(1, &_arrayID);
}

static constexpr GLenum toGLType(BufferElement::Type type) {
  switch (type) {
  case BufferElement::Type::Float:
  case BufferElement::Type::Float2:
  case BufferElement::Type::Float3:
  case BufferElement::Type::Float4:
  case BufferElement::Type::Mat3:
  case BufferElement::Type::Mat4:
    return GL_FLOAT;
  case BufferElement::Type::Int:
  case BufferElement::Type::Int2:
  case BufferElement::Type::Int3:
  case BufferElement::Type::Int4:
    return GL_INT;
  case BufferElement::Type::Bool:
    return GL_BOOL;
  default:
    break;
  }

  assert(false);
  return 0;
}

void VertexArray::setBuffers(const OpenGL::VertexBuffer& vertexBuf,
                             const OpenGL::IndexBuffer& indexBuf) {
  // Bind vertex buf
  const auto& layout = vertexBuf.layout();
  assert(layout.elements().size());

  glBindVertexArray(_arrayID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuf.bufferID());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf.bufferID());

  for (const auto& e : layout.elements()) {
    auto index = static_cast<int>(&e - layout.elements().data());
    glVertexAttribPointer(index,
                          e.componentCount(),
                          toGLType(e.type),
                          GL_FALSE,
                          layout.stride(),
                          reinterpret_cast<void*>(e.offset));
    glEnableVertexAttribArray(index);
  }
}
} // namespace Engine::Renderer::OpenGL