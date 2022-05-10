#include <libcore/renderer/BufferLayout.hh>
#include <libcore/renderer/opengl/VertexArray.hh>

namespace Engine::Renderer {
OpenGLVertexArray::OpenGLVertexArray() {
  glGenVertexArrays(1, &_arrayID);
}

void OpenGLVertexArray::bind() const {
  glBindVertexArray(_arrayID);
}

void OpenGLVertexArray::unbind() const {
  glBindVertexArray(0);
}

static GLenum toOpenGLType(Shader::DataType type) {
  using namespace Renderer::Buffer;

  switch (type) {
  case Shader::DataType::Float:
  case Shader::DataType::Float2:
  case Shader::DataType::Float3:
  case Shader::DataType::Float4:
  case Shader::DataType::Mat3:
  case Shader::DataType::Mat4:
    return GL_FLOAT;
  case Shader::DataType::Int:
  case Shader::DataType::Int2:
  case Shader::DataType::Int3:
  case Shader::DataType::Int4:
    return GL_INT;
  case Shader::DataType::Bool:
    return GL_BOOL;
  default:
    break;
  }

  assert(false);
  return 0;
}

void OpenGLVertexArray::setBuffers(const Buffer::Vertex& vertexBuf,
                                   const Buffer::Index& indexBuf) {
  // Bind vertex buf
  const auto& layout = vertexBuf.layout();
  assert(layout.elements().size());

  glBindVertexArray(_arrayID);
  vertexBuf.bind();
  indexBuf.bind();

  for (const auto& e : layout.elements()) {
    auto index = static_cast<int>(&e - layout.elements().data());
    glVertexAttribPointer(index,
                          e.componentCount(),
                          toOpenGLType(e.type),
                          GL_FALSE,
                          layout.stride(),
                          reinterpret_cast<void*>(e.offset));
    glEnableVertexAttribArray(index);
  }
}
} // namespace Engine::Renderer