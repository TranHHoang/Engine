#pragma once
#include <cstdint>

#include <libcore/renderer/opengl/Buffer.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer::OpenGL {
class VertexArray {
public:
  VertexArray();

  GLuint arrayID() const { return _arrayID; }

  void setBuffers(const OpenGL::VertexBuffer& vertexBuf,
                  const OpenGL::IndexBuffer& indexBuf);

private:
  GLuint _arrayID;
};
} // namespace Engine::Renderer::OpenGL