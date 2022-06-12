#pragma once
#include <cstdint>

#include <libcore/renderer/Buffer.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer::OpenGL {
class VertexArray {
public:
  VertexArray();
  void bind() const;
  void unbind() const;
  void setBuffers(const VertexBuffer& vertexBuf, const IndexBuffer& indexBuf);

private:
  GLuint _arrayID;
};
} // namespace Engine::Renderer::OpenGL